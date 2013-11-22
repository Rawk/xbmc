#pragma once
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

#include "Archive.h"
#include "ISerializable.h"
#include <vector>


class CStreamDetail : public IArchivable, public ISerializable
{
public:
  enum StreamType {
    VIDEO,
    AUDIO,
    SUBTITLE
  };

  virtual void Archive(CArchive& ar) = 0;
  virtual void Serialize(CVariant& value) const = 0;
};


class CStreamDetailVideo : public CStreamDetail
{
public:
  CStreamDetailVideo(int width = 0, int height = 0, float aspect = 0,
            int duration = 0, const CStdString &codec = CStdString(),
            const std::string &stereoMode = std::string());
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

  int m_iWidth;
  int m_iHeight;
  float m_fAspect;
  int m_iDuration;
  CStdString m_strCodec;
  std::string m_strStereoMode;
};

bool operator< (const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);
bool operator==(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);
bool operator!=(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);

class CStreamDetailAudio : public CStreamDetail
{
public:
  CStreamDetailAudio(int channels = -1, const CStdString language = CStdString(),
                  const CStdString codec = CStdString());
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

  int m_iChannels;
  CStdString m_strLanguage;
  CStdString m_strCodec;
};

bool operator< (const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);
bool operator==(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);
bool operator!=(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);


class CStreamDetailSubtitle : public CStreamDetail
{
public:
  CStreamDetailSubtitle(const CStdString &strLanguage = CStdString());
  CStreamDetailSubtitle& operator=(const CStreamDetailSubtitle &that);
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

  CStdString m_strLanguage;
};

bool operator==(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs);
bool operator!=(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs);

class CStreamDetails : public IArchivable, public ISerializable
{
public:
  CStreamDetails() { };
  CStreamDetails(const CStreamDetails &that);
  CStreamDetails& operator=(const CStreamDetails &that);

  static CStdString VideoDimsToResolutionDescription(int iWidth, int iHeight);
  static CStdString VideoAspectToAspectDescription(float fAspect);

  bool HasItems(void) const;
  int GetVideoStreamCount(void) const;
  int GetAudioStreamCount(void) const;
  int GetSubtitleStreamCount(void) const;
  const CStreamDetail* GetNthStream(CStreamDetail::StreamType type, unsigned int idx) const;
  const CStreamDetailVideo* GetNthVideoStream(unsigned int idx) const;
  const CStreamDetailAudio* GetNthAudioStream(unsigned int idx) const;
  const CStreamDetailSubtitle* GetNthSubtitleStream(unsigned int idx) const;

  const std::vector<CStreamDetailVideo>& GetVideoStreams() const;
  const std::vector<CStreamDetailAudio>& GetAudioStreams() const;
  const std::vector<CStreamDetailSubtitle>& GetSubtitleStreams() const;

  CStdString GetVideoCodec(unsigned int idx = 0) const;
  float GetVideoAspect(unsigned int idx = 0) const;
  int GetVideoWidth(unsigned int idx = 0) const;
  int GetVideoHeight(unsigned int idx = 0) const;
  int GetVideoDuration(unsigned int idx = 0) const;
  std::string GetStereoMode(unsigned int idx = 0) const;

  CStdString GetAudioCodec(unsigned int idx = 0) const;
  CStdString GetAudioLanguage(unsigned int idx = 0) const;
  int GetAudioChannels(unsigned int idx = 0) const;

  CStdString GetSubtitleLanguage(unsigned int idx = 0) const;

  void AddStream(const CStreamDetailAudio &item);
  void AddStream(const CStreamDetailVideo &item);
  void AddStream(const CStreamDetailSubtitle &item);
  void Reset(void);

  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

private:
  std::vector<CStreamDetailVideo> m_vecVideos;
  std::vector<CStreamDetailAudio> m_vecAudios;
  std::vector<CStreamDetailSubtitle> m_vecSubtitles;
};

bool operator==(const CStreamDetails &lhs, const CStreamDetails &rhs);
bool operator!=(const CStreamDetails &lhs, const CStreamDetails &rhs);

