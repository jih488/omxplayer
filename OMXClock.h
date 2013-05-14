/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef _AVCLOCK_H_
#define _AVCLOCK_H_

#include "DllAvFormat.h"

#include "OMXCore.h"

#define DVD_TIME_BASE 1000000
#define DVD_NOPTS_VALUE    (-1LL<<52) // should be possible to represent in both double and __int64

#define DVD_TIME_TO_SEC(x)  ((int)((double)(x) / DVD_TIME_BASE))
#define DVD_TIME_TO_MSEC(x) ((int)((double)(x) * 1000 / DVD_TIME_BASE))
#define DVD_SEC_TO_TIME(x)  ((double)(x) * DVD_TIME_BASE)
#define DVD_MSEC_TO_TIME(x) ((double)(x) * DVD_TIME_BASE / 1000)

#define DVD_PLAYSPEED_PAUSE       0       // frame stepping
#define DVD_PLAYSPEED_NORMAL      1000

#ifdef OMX_SKIP64BIT
static inline OMX_TICKS ToOMXTime(int64_t pts)
{
  OMX_TICKS ticks;
  ticks.nLowPart = pts;
  ticks.nHighPart = pts >> 32;
  return ticks;
}
static inline int64_t FromOMXTime(OMX_TICKS ticks)
{
  int64_t pts = ticks.nLowPart | ((uint64_t)(ticks.nHighPart) << 32);
  return pts;
}
#else
#define FromOMXTime(x) (x)
#define ToOMXTime(x) (x)
#endif

class OMXClock
{
protected:
  double            m_video_clock;
  double            m_audio_clock;
  bool              m_pause;
  bool              m_has_video;
  bool              m_has_audio;
  int               m_play_speed;
  pthread_mutex_t   m_lock;
private:
  COMXCoreComponent m_omx_clock;
  DllAvFormat       m_dllAvFormat;
public:
  OMXClock();
  ~OMXClock();
  void Lock();
  void UnLock();
  bool OMXInitialize(bool has_video, bool has_audio);
  void Deinitialize();
  bool OMXIsPaused() { return m_pause; };
  bool OMXStop(bool lock = true);
  bool OMXStart(double pts, bool lock = true);
  bool OMXStep(bool lock = true);
  bool OMXReset(bool lock = true);
  double OMXMediaTime(bool lock = true);
  double OMXClockAdjustment(bool lock = true);
  bool OMXMediaTime(double pts, bool fixPreroll = true, bool lock = true);
  bool OMXPause(bool lock = true);
  bool OMXResume(bool lock = true);
  bool OMXSpeed(int speed, bool lock = true, bool pause_resume = false);
  int  OMXPlaySpeed() { return m_play_speed; };
  COMXCoreComponent *GetOMXClock();
  bool OMXStateExecute(bool lock = true);
  void OMXStateIdle(bool lock = true);
  bool HDMIClockSync(bool lock = true);
  bool HasVideo() { return m_has_video; };
  bool HasAudio() { return m_has_audio; };
  static void OMXSleep(unsigned int dwMilliSeconds);
};

#endif
