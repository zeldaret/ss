#ifndef M_VIDEO_H
#define M_VIDEO_H

#include <egg/core/eggVideo.h>

namespace mVideo {

// 0x80065590
void create();

extern EGG::Video *m_video;

extern f32 l_fbAspect;

} // namespace mVideo

#endif
