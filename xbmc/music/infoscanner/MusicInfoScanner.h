#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include "threads/Thread.h"
#include "music/MusicDatabase.h"
#include "MusicAlbumInfo.h"
#include "MusicInfoScraper.h"

class CAlbum;
class CArtist;
class CGUIDialogProgressBarHandle;

namespace MUSIC_INFO
{
/*! \brief return values from the information lookup functions
 */
enum INFO_RET 
{ 
  INFO_CANCELLED,
  INFO_ERROR,
  INFO_NOT_NEEDED,
  INFO_HAVE_ALREADY,
  INFO_NOT_FOUND,
  INFO_ADDED 
};

class CMusicInfoScanner : CThread, public IRunnable
{
public:
  /*! \brief Flags for controlling the scanning process
   */
  enum SCAN_FLAGS { SCAN_NORMAL     = 0,
                    SCAN_ONLINE     = 1 << 0,
                    SCAN_BACKGROUND = 1 << 1,
                    SCAN_RESCAN     = 1 << 2 };

  CMusicInfoScanner();
  virtual ~CMusicInfoScanner();

  void Start(const CStdString& strDirectory, int flags);
  void FetchAlbumInfo(const CStdString& strDirectory, bool refresh=false);
  void FetchArtistInfo(const CStdString& strDirectory, bool refresh=false);
  bool IsScanning();
  void Stop();

  //! \brief Set whether or not to show a progress dialog
  void ShowDialog(bool show) { m_showDialog = show; }

  /*! \brief Categorize FileItems into Albums, Songs, and Artists
   This takes a list of FileItems and turns it into a tree of Albums,
   Artists, and Songs.
   Albums are defined uniquely by the album name and album artist.
   
   \param songs [in/out] list of songs to categorise - albumartist field may be altered.
   \param albums [out] albums found within these songs.
   */
  static void FileItemsToAlbums(CFileItemList& items, VECALBUMS& albums, MAPSONGS* songsMap = NULL);

  /*! \brief Fixup albums and songs
   
   If albumartist is not available in a song, we determine it from the
   common portion of each song's artist list.
   
   eg the common artist for
   Bob Dylan / Tom Petty / Roy Orbison
   Bob Dylan / Tom Petty
   would be "Bob Dylan / Tom Petty".
   
   If all songs that share an album
   1. have a non-empty album name
   2. have at least two different primary artists
   3. have no album artist set
   4. and no track numbers overlap
   we assume it is a various artists album, and set the albumartist field accordingly.
   
   */
  static void FixupAlbums(VECALBUMS &albums);

  /*! \brief Find art for albums
   Based on the albums in the folder, finds whether we have unique album art
   and assigns to the album if we do.

   In order of priority:
    1. If there is a single album in the folder, then the folder art is assigned to the album.
    2. We find the art for each song. A .tbn file takes priority over embedded art.
    3. If we have a unique piece of art for all songs in the album, we assign that to the album
       and remove that art from each song so that they inherit from the album.
    4. If there is not a unique piece of art for each song, then no art is assigned
       to the album.

   \param albums [in/out] list of albums to categorise - art field may be altered.
   \param path [in] path containing albums.
   */
  static void FindArtForAlbums(VECALBUMS &albums, const CStdString &path);

  INFO_RET DownloadAlbumInfo(const CAlbum& album, ADDON::ScraperPtr& scraper, MUSIC_GRABBER::CMusicAlbumInfo& albumInfo, CGUIDialogProgress* pDialog = NULL);
  INFO_RET DownloadArtistInfo(const CArtist& artist, ADDON::ScraperPtr& scraper, MUSIC_GRABBER::CMusicArtistInfo& artistInfo, CGUIDialogProgress* pDialog = NULL);

  std::map<std::string, std::string> GetArtistArtwork(long id, const CArtist *artist = NULL);
protected:
  virtual void Process();

  /*! \brief Scan in the ID3/Ogg/FLAC tags for a bunch of FileItems
   Given a list of FileItems, scan in the tags for those FileItems
   and populate a new FileItemList with the files that were successfully scanned.
   Any files which couldn't be scanned (no/bad tags) are discarded in the process.
   \param items [in] list of FileItems to scan
   \param scannedItems [in] list to populate with the scannedItems
   */
  int RetrieveMusicInfo(const CStdString& strDirectory, CFileItemList& items);

  /*! \brief Scan in the ID3/Ogg/FLAC tags for a bunch of FileItems
    Given a list of FileItems, scan in the tags for those FileItems
   and populate a new FileItemList with the files that were successfully scanned.
   Any files which couldn't be scanned (no/bad tags) are discarded in the process.
   \param items [in] list of FileItems to scan
   \param scannedItems [in] list to populate with the scannedItems
   */
  INFO_RET ScanTags(const CFileItemList& items, CFileItemList& scannedItems);
  int GetPathHash(const CFileItemList &items, CStdString &hash);
  void GetAlbumArtwork(long id, const CAlbum &artist);

  bool DoScan(const CStdString& strDirectory);

  virtual void Run();
  int CountFiles(const CFileItemList& items, bool recursive);
  int CountFilesRecursively(const CStdString& strPath);

  /*! \brief Resolve a MusicBrainzID to a URL
   If we have a MusicBrainz ID for an artist or album, 
   resolve it to an MB URL and set up the scrapers accordingly.
   
   \param preferredScraper [in] A ScraperPtr to the preferred album/artist scraper.
   \param musicBrainzURL [out] will be populated with the MB URL for the artist/album.
   */
  bool ResolveMusicBrainz(const CStdString strMusicBrainzID, ADDON::ScraperPtr &preferredScraper, MUSIC_GRABBER::CMusicInfoScraper &musicInfoScraper, CScraperUrl &musicBrainzURL);

protected:
  bool m_showDialog;
  CGUIDialogProgressBarHandle* m_handle;
  int m_currentItem;
  int m_itemCount;
  bool m_bRunning;
  bool m_bCanInterrupt;
  bool m_needsCleanup;
  int m_scanType; // 0 - load from files, 1 - albums, 2 - artists
  CMusicDatabase m_musicDatabase;

  std::map<CAlbum, CAlbum> m_albumCache;
  std::map<CArtistCredit, CArtist> m_artistCache;

  std::set<std::string> m_pathsToScan;
  int m_flags;
  CThread m_fileCountReader;
};
}
