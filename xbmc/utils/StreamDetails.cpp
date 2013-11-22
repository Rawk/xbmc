/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <math.h>
#include "StreamDetails.h"
#include "StreamUtils.h"
#include "Variant.h"

const float VIDEOASPECT_EPSILON = 0.025f;


CStreamDetailVideo::CStreamDetailVideo(int width, int height, float aspect,
        int duration, const CStdString &codec, const std::string &stereoMode) :
  m_iWidth(width), m_iHeight(height), m_iDuration(duration), m_strCodec(codec),
  m_strStereoMode(stereoMode)
{
  if (aspect == 0.0f && m_iHeight != 0)
    m_fAspect = (float)m_iWidth / m_iHeight;
  else
    m_fAspect = aspect;
}

void CStreamDetailVideo::Archive(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << m_strCodec;
    ar << m_fAspect;
    ar << m_iHeight;
    ar << m_iWidth;
    ar << m_iDuration;
    ar << m_strStereoMode;
  }
  else
  {
    ar >> m_strCodec;
    ar >> m_fAspect;
    ar >> m_iHeight;
    ar >> m_iWidth;
    ar >> m_iDuration;
    ar >> m_strStereoMode;
  }
}
void CStreamDetailVideo::Serialize(CVariant& value) const
{
  value["codec"] = m_strCodec;
  value["aspect"] = m_fAspect;
  value["height"] = m_iHeight;
  value["width"] = m_iWidth;
  value["duration"] = m_iDuration;
  value["stereomode"] = m_strStereoMode;
}

bool operator< (const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs)
{
  // Best video stream is that with the most pixels
  return (lhs.m_iWidth * lhs.m_iHeight) < (rhs.m_iWidth * rhs.m_iHeight);
}

bool operator==(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs)
{
  if (&lhs == &rhs)
    return true;

  return lhs.m_iWidth == rhs.m_iWidth &&
         lhs.m_iHeight == rhs.m_iHeight &&
         lhs.m_iDuration == rhs.m_iDuration &&
         lhs.m_strCodec == rhs.m_strCodec &&
         lhs.m_strStereoMode == rhs.m_strStereoMode &&
         fabs(lhs.m_fAspect - rhs.m_fAspect) <= VIDEOASPECT_EPSILON;
}

bool operator!=(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs)
{
  return !(lhs == rhs);
}

CStreamDetailAudio::CStreamDetailAudio(int channels, const CStdString language,
                                        const CStdString codec) :
  m_iChannels(channels), m_strLanguage(language), m_strCodec(codec)
{
}

void CStreamDetailAudio::Archive(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << m_strCodec;
    ar << m_strLanguage;
    ar << m_iChannels;
  }
  else
  {
    ar >> m_strCodec;
    ar >> m_strLanguage;
    ar >> m_iChannels;
  }
}

void CStreamDetailAudio::Serialize(CVariant& value) const
{
  value["codec"] = m_strCodec;
  value["language"] = m_strLanguage;
  value["channels"] = m_iChannels;
}

bool operator< (const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs)
{
  // First choice is the thing with the most channels
  // In case of a tie, revert to codec priority
  if (lhs.m_iChannels == rhs.m_iChannels)
    return StreamUtils::GetCodecPriority(lhs.m_strCodec) <
           StreamUtils::GetCodecPriority(rhs.m_strCodec);

  return lhs.m_iChannels < rhs.m_iChannels;
}

bool operator==(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs)
{
  if (&lhs == &rhs)
    return true;

  return lhs.m_iChannels == rhs.m_iChannels &&
         lhs.m_strLanguage == rhs.m_strLanguage &&
         lhs.m_strCodec == rhs.m_strCodec;
}

bool operator!=(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs)
{
  return !(lhs == rhs);
}

CStreamDetailSubtitle::CStreamDetailSubtitle(const CStdString &strLanguage)
  : m_strLanguage(strLanguage)
{
}

void CStreamDetailSubtitle::Archive(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << m_strLanguage;
  }
  else
  {
    ar >> m_strLanguage;
  }
}

void CStreamDetailSubtitle::Serialize(CVariant& value) const
{
  value["language"] = m_strLanguage;
}

CStreamDetailSubtitle& CStreamDetailSubtitle::operator=(const CStreamDetailSubtitle &that)
{
  if (this != &that)
    this->m_strLanguage = that.m_strLanguage;

  return *this;
}

bool operator==(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs)
{
  if (&lhs == &rhs)
    return true;

  return lhs.m_strLanguage == rhs.m_strLanguage;
}

bool operator!=(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs)
{
  return !(lhs == rhs);
}

CStreamDetails& CStreamDetails::operator=(const CStreamDetails &that)
{
  if (this != &that)
  {
    m_vecVideos = that.m_vecVideos;
    m_vecAudios = that.m_vecAudios;
    m_vecSubtitles = that.m_vecSubtitles;
  }

  return *this;
}

bool operator==(const CStreamDetails &lhs, const CStreamDetails &rhs)
{
  if (&lhs == &rhs)
    return true;

  return lhs.GetVideoStreams() == rhs.GetVideoStreams() &&
         lhs.GetAudioStreams() == rhs.GetAudioStreams() &&
         lhs.GetSubtitleStreams() == rhs.GetSubtitleStreams();
}

bool operator!=(const CStreamDetails &lhs, const CStreamDetails &rhs)
{
  return !(lhs == rhs);
}

bool CStreamDetails::HasItems() const
{
  return !m_vecVideos.empty() || !m_vecAudios.empty()
          || !m_vecSubtitles.empty();
}

int CStreamDetails::GetVideoStreamCount(void) const
{
  return m_vecVideos.size();
}

int CStreamDetails::GetAudioStreamCount(void) const
{
  return m_vecAudios.size();
}

int CStreamDetails::GetSubtitleStreamCount(void) const
{
  return m_vecSubtitles.size();
}

CStreamDetails::CStreamDetails(const CStreamDetails &that)
{
  *this = that;
}

void CStreamDetails::AddStream(const CStreamDetailVideo &item)
{
  m_vecVideos.push_back(item);
}

void CStreamDetails::AddStream(const CStreamDetailAudio &item)
{
  m_vecAudios.push_back(item);
}

void CStreamDetails::AddStream(const CStreamDetailSubtitle &item)
{
  m_vecSubtitles.push_back(item);
}

void CStreamDetails::Reset(void)
{
  m_vecVideos.clear();
  m_vecAudios.clear();
  m_vecSubtitles.clear();
}

const CStreamDetail* CStreamDetails::GetNthStream(CStreamDetail::StreamType type,
          unsigned int idx) const
{
  switch (type)
  {
  case CStreamDetail::VIDEO:
    return GetNthVideoStream(idx);
  case CStreamDetail::AUDIO:
    return GetNthAudioStream(idx);
  case CStreamDetail::SUBTITLE:
    return GetNthSubtitleStream(idx);
  }

  return NULL;
}

const CStreamDetailVideo* CStreamDetails::GetNthVideoStream(unsigned int idx) const
{
  if (m_vecVideos.empty() || idx > m_vecVideos.size())
    return NULL;

  if (idx == 0)
    return &(*max_element(m_vecVideos.begin(), m_vecVideos.end()));

  return &m_vecVideos[idx - 1];
}

const CStreamDetailAudio* CStreamDetails::GetNthAudioStream(unsigned int idx) const
{
  if (m_vecAudios.empty() || idx > m_vecAudios.size())
    return NULL;

  if (idx == 0)
    return &(*max_element(m_vecAudios.begin(), m_vecAudios.end()));

  return &m_vecAudios[idx - 1];
}

const CStreamDetailSubtitle * CStreamDetails::GetNthSubtitleStream(unsigned int idx) const
{
  if (m_vecSubtitles.empty() || idx > m_vecSubtitles.size())
    return NULL;

  if (idx == 0) // Currently no priority order for subs, just take the first
    return &m_vecSubtitles.front();

  return &m_vecSubtitles[idx - 1];
}

const std::vector<CStreamDetailVideo>& CStreamDetails::GetVideoStreams() const
{
  return m_vecVideos;
}

const std::vector<CStreamDetailAudio>& CStreamDetails::GetAudioStreams() const
{
  return m_vecAudios;
}

const std::vector<CStreamDetailSubtitle>& CStreamDetails::GetSubtitleStreams() const
{
  return m_vecSubtitles;
}

CStdString CStreamDetails::GetVideoCodec(unsigned int idx) const
{
  const CStreamDetailVideo *item = GetNthVideoStream(idx);
  if (item)
    return item->m_strCodec;
  else
    return "";
}

float CStreamDetails::GetVideoAspect(unsigned int idx) const
{
  const CStreamDetailVideo *item = GetNthVideoStream(idx);
  if (item)
    return item->m_fAspect;
  else
    return 0.0;
}

int CStreamDetails::GetVideoWidth(unsigned int idx) const
{
  const CStreamDetailVideo *item = GetNthVideoStream(idx);
  if (item)
    return item->m_iWidth;
  else
    return 0;
}

int CStreamDetails::GetVideoHeight(unsigned int idx) const
{
  const CStreamDetailVideo *item = GetNthVideoStream(idx);
  if (item)
    return item->m_iHeight;
  else
    return 0;
}

int CStreamDetails::GetVideoDuration(unsigned int idx) const
{
  const CStreamDetailVideo *item = GetNthVideoStream(idx);
  if (item)
    return item->m_iDuration;
  else
    return 0;
}

std::string CStreamDetails::GetStereoMode(unsigned int idx) const
{
  CStreamDetailVideo *item = (CStreamDetailVideo *)GetNthStream(CStreamDetail::VIDEO, idx);
  if (item)
    return item->m_strStereoMode;
  else
    return "";
}

CStdString CStreamDetails::GetAudioCodec(unsigned int idx) const
{
  const CStreamDetailAudio *item = GetNthAudioStream(idx);
  if (item)
    return item->m_strCodec;
  else
    return "";
}

CStdString CStreamDetails::GetAudioLanguage(unsigned int idx) const
{
  const CStreamDetailAudio *item = GetNthAudioStream(idx);
  if (item)
    return item->m_strLanguage;
  else
    return "";
}

int CStreamDetails::GetAudioChannels(unsigned int idx) const
{
  const CStreamDetailAudio *item = GetNthAudioStream(idx);
  if (item)
    return item->m_iChannels;
  else
    return -1;
}

CStdString CStreamDetails::GetSubtitleLanguage(unsigned int idx) const
{
  const CStreamDetailSubtitle *item = GetNthSubtitleStream(idx);
  if (item)
    return item->m_strLanguage;
  else
    return "";
}

void CStreamDetails::Archive(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << m_vecVideos;
    ar << m_vecAudios;
    ar << m_vecSubtitles;
  }
  else
  {
    ar >> m_vecVideos;
    ar >> m_vecAudios;
    ar >> m_vecSubtitles;
  }
}

void CStreamDetails::Serialize(CVariant& value) const
{
  CVariant v;

  // make sure these properties are always present
  value["video"] = CVariant(CVariant::VariantTypeArray);
  for (std::vector<CStreamDetailVideo>::const_iterator it = m_vecVideos.begin();
        it != m_vecVideos.end(); ++it)
  {
    v.clear();
    it->Serialize(v);
    value["video"].push_back(v);
  }

  // make sure these properties are always present
  value["audio"] = CVariant(CVariant::VariantTypeArray);
  for (std::vector<CStreamDetailAudio>::const_iterator it = m_vecAudios.begin();
        it != m_vecAudios.end(); ++it)
  {
    v.clear();
    it->Serialize(v);
    value["audio"].push_back(v);
  }

  // make sure these properties are always present
  value["subtitle"] = CVariant(CVariant::VariantTypeArray);
  for (std::vector<CStreamDetailSubtitle>::const_iterator it = m_vecSubtitles.begin();
        it != m_vecSubtitles.end(); ++it)
  {
    v.clear();
    it->Serialize(v);
    value["subtitle"].push_back(v);
  }
}

CStdString CStreamDetails::VideoDimsToResolutionDescription(int iWidth, int iHeight)
{
  if (iWidth == 0 || iHeight == 0)
    return "";
  else if (iWidth <= 720 && iHeight <= 480)
    return "480";
  // 720x576 (PAL) (768 when rescaled for square pixels)
  else if (iWidth <= 768 && iHeight <= 576)
    return "576";
  // 960x540 (sometimes 544 which is multiple of 16)
  else if (iWidth <= 960 && iHeight <= 544)
    return "540";
  // 1280x720
  else if (iWidth <= 1280 && iHeight <= 720)
    return "720";
  // 1920x1080
  else if (iWidth <= 1920 && iHeight <= 1080)
    return "1080";
  // 4K
  else
    return "4K";
}

CStdString CStreamDetails::VideoAspectToAspectDescription(float fAspect)
{
  if (fAspect == 0.0f)
    return "";

  // Given that we're never going to be able to handle every single possibility in
  // aspect ratios, particularly when cropping prior to video encoding is taken into account
  // the best we can do is take the "common" aspect ratios, and return the closest one available.
  // The cutoffs are the geometric mean of the two aspect ratios either side.
  if (fAspect < 1.3499f) // sqrt(1.33*1.37)
    return "1.33";
  else if (fAspect < 1.5080f) // sqrt(1.37*1.66)
    return "1.37";
  else if (fAspect < 1.7190f) // sqrt(1.66*1.78)
    return "1.66";
  else if (fAspect < 1.8147f) // sqrt(1.78*1.85)
    return "1.78";
  else if (fAspect < 2.0174f) // sqrt(1.85*2.20)
    return "1.85";
  else if (fAspect < 2.2738f) // sqrt(2.20*2.35)
    return "2.20";
  else if (fAspect < 2.3749f) // sqrt(2.35*2.40)
    return "2.35";
  else if (fAspect < 2.4739f) // sqrt(2.40*2.55)
    return "2.40";
  else if (fAspect < 2.6529f) // sqrt(2.55*2.76)
    return "2.55";
  return "2.76";
}
