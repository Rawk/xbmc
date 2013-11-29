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
            int duration = 0, const std::string &codec = std::string(),
            const std::string &stereoMode = std::string());
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;
/*
  int GetWidth() const { return m_iWidth; }
  void SetWidth(int width) { m_iWidth = width; }
  int GetHeight() const { return m_iHeight; }
  void SetHeight(int height) { m_iHeight = height; }
  float GetAspect() const { return m_fAspect; }
  void SetAspect(float aspect) { m_fAspect = aspect; }
  int GetDuration() const { return m_iDuration; }
  void SetDuration(int duration) { m_iDuration = duration; }
  const std::string& GetCodec() const { return m_strCodec; }
  void SetCodec(const std::string &codec) { m_strCodec = codec; }
  const std::string& GetStereoMode() const { return m_strStereoMode; }
  void SetStereoMode(const std::string &stereoMode) { m_strStereoMode = stereoMode; }
*/
  std::string GetResolutionDescription() const;
  std::string GetAspectDescription() const;

  static const CStreamDetailVideo s_none;

//private:
  int m_iWidth;
  int m_iHeight;
  float m_fAspect;
  int m_iDuration;
  std::string m_strCodec;
  std::string m_strStereoMode;
};

bool operator< (const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);
bool operator==(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);
bool operator!=(const CStreamDetailVideo &lhs, const CStreamDetailVideo &rhs);

class CStreamDetailAudio : public CStreamDetail
{
public:
  CStreamDetailAudio(int channels = -1, const std::string language = std::string(),
                  const std::string codec = std::string());
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

/*
  int GetChannels() const { return m_iChannels; }
  void SetChannels(int channels) { m_iChannels = channels; }
  const std::string& GetLanguage() const { return m_strLanguage; }
  void SetLanguage(const std::string &language) { m_strLanguage = language; }
  const std::string& GetCodec() const { return m_strCodec; }
  void SetCodec(const std::string &codec) { m_strCodec = codec; }
*/
  static const CStreamDetailAudio s_none;

//private:
  int m_iChannels;
  std::string m_strLanguage;
  std::string m_strCodec;
};

bool operator< (const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);
bool operator==(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);
bool operator!=(const CStreamDetailAudio &lhs, const CStreamDetailAudio &rhs);


class CStreamDetailSubtitle : public CStreamDetail
{
public:
  CStreamDetailSubtitle(const std::string &strLanguage = std::string());
  CStreamDetailSubtitle& operator=(const CStreamDetailSubtitle &that);
  virtual void Archive(CArchive& ar);
  virtual void Serialize(CVariant& value) const;

/*
  const std::string& GetLanguage() const { return m_strLanguage; }
  void SetLanguage(const std::string &language) { m_strLanguage = language; }
*/
  static const CStreamDetailSubtitle s_none;

//private:
  std::string m_strLanguage;
};

bool operator==(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs);
bool operator!=(const CStreamDetailSubtitle &lhs, const CStreamDetailSubtitle &rhs);

class CStreamDetails : public IArchivable, public ISerializable
{
public:
  CStreamDetails() { };
  CStreamDetails(const CStreamDetails &that);
  CStreamDetails& operator=(const CStreamDetails &that);

  static std::string VideoDimsToResolutionDescription(int iWidth, int iHeight);
  static std::string VideoAspectToAspectDescription(float fAspect);

  bool HasStreams(void) const;
  bool HasVideo(void) const;
  bool HasAudio(void) const;
  bool HasSubtitle(void) const;
  int GetVideoCount(void) const;
  int GetAudioCount(void) const;
  int GetSubtitleCount(void) const;
  CStreamDetailVideo& GetBestVideo();
  CStreamDetailAudio& GetBestAudio();
  CStreamDetailSubtitle& GetBestSubtitle();
  const CStreamDetailVideo& GetBestVideo() const;
  const CStreamDetailAudio& GetBestAudio() const;
  const CStreamDetailSubtitle& GetBestSubtitle() const;

  typedef std::vector<CStreamDetailVideo>::const_iterator VideoConstIt;
  typedef std::vector<CStreamDetailAudio>::const_iterator AudioConstIt;
  typedef std::vector<CStreamDetailSubtitle>::const_iterator SubtitleConstIt;
  VideoConstIt VideosBegin() const { return m_vecVideos.begin(); }
  AudioConstIt AudiosBegin() const { return m_vecAudios.begin(); }
  SubtitleConstIt SubtitlesBegin() const { return m_vecSubtitles.begin(); }
  VideoConstIt VideosEnd() const { return m_vecVideos.end(); }
  AudioConstIt AudiosEnd() const { return m_vecAudios.end(); }
  SubtitleConstIt SubtitlesEnd() const { return m_vecSubtitles.end(); }
  const std::vector<CStreamDetailVideo>& GetVideos() const;
  const std::vector<CStreamDetailAudio>& GetAudios() const;
  const std::vector<CStreamDetailSubtitle>& GetSubtitles() const;

/*  std::string GetVideoCodec(unsigned int idx = 0) const;
  float GetVideoAspect(unsigned int idx = 0) const;
  int GetVideoWidth(unsigned int idx = 0) const;
  int GetVideoHeight(unsigned int idx = 0) const;
  int GetVideoDuration(unsigned int idx = 0) const;
  std::string GetStereoMode(unsigned int idx = 0) const;

  std::string GetAudioCodec(unsigned int idx = 0) const;
  std::string GetAudioLanguage(unsigned int idx = 0) const;
  int GetAudioChannels(unsigned int idx = 0) const;

  std::string GetSubtitleLanguage(unsigned int idx = 0) const;*/

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

