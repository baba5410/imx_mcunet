#ifndef V4L2_DRIVER_H
#define V4L2_DRIVER_H

#include <linux/videodev2.h>
#include <string>
#include <vector>

struct CamBufInfo {
  void *start;
  size_t length;
};

class V4L2Driver {
public:
  V4L2Driver();
  ~V4L2Driver();

  bool openDevice(const std::string &device);
  bool initFormat(int width, int height);
  bool initBuffers();
  bool startStreaming();
  bool stopStreaming();

  // 获取一帧数据的原始指针
  unsigned short *getFrame(int &index);
  void releaseFrame(int index);

private:
  int fd = -1;
  int m_width, m_height;
  std::vector<CamBufInfo> buffers;
};

#endif
