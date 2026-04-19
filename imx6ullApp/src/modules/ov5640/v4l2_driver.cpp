#include "v4l2_driver.h"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

V4L2Driver::V4L2Driver() {}
V4L2Driver::~V4L2Driver() {
  stopStreaming();
    // 2. 显式解除内存映射 (归还 DMA 缓冲区给内核)
    for (auto &buf : buffers) {
        if (buf.start != nullptr && buf.start != MAP_FAILED) {
            munmap(buf.start, buf.length);
        }
    }
    buffers.clear();
    // 3. 关闭设备
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}
bool V4L2Driver::openDevice(const std::string &device) {
  fd = open(device.c_str(), O_RDWR);
  return fd != -1;
}

bool V4L2Driver::initFormat(int width, int height) {
  struct v4l2_format fmt = {0};
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = width;
  fmt.fmt.pix.height = height;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;

  if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
    return false;
  m_width = fmt.fmt.pix.width;
  m_height = fmt.fmt.pix.height;
  return true;
}

bool V4L2Driver::initBuffers() {
  struct v4l2_requestbuffers req = {0};
  req.count = 3;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0)
    return false;

  for (int i = 0; i < 3; ++i) {
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0)
      return false;

    void *ptr = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                     buf.m.offset);
    buffers.push_back({ptr, buf.length});
    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
      return false;
  }
  return true;
}

bool V4L2Driver::startStreaming() {
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return ioctl(fd, VIDIOC_STREAMON, &type) >= 0;
}

unsigned short *V4L2Driver::getFrame(int &index) {
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0)
    return nullptr;
  index = buf.index;
  return static_cast<unsigned short *>(buffers[index].start);
}

void V4L2Driver::releaseFrame(int index) {
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = index;
  ioctl(fd, VIDIOC_QBUF, &buf);
}
bool V4L2Driver::stopStreaming() {
    if (fd == -1) return false;

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
        std::cerr << "V4L2: Failed to stop streaming" << std::endl;
        return false;
    }
    return true;
}
