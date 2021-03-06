/**
* @file CUOMapList.h
*
*/

#ifndef _INC_CUOMAPLIST_H
#define _INC_CUOMAPLIST_H

#include "../../common/common.h"

// All these structures must be byte packed.
#if defined(_WIN32) && defined(_MSC_VER)
	// Microsoft dependant pragma
	#pragma pack(1)
	#define PACK_NEEDED
#else
	// GCC based compiler you can add:
	#define PACK_NEEDED __attribute__ ((packed))
#endif


class CServerMapDiffCollection;

extern class CUOMapList
{
#define MAP_SUPPORTED_QTY 256

    friend struct CUOInstall;
    friend class  CWorld;
    friend class  CServerMap;
    friend class  CServerMapBlock;

protected:
    bool m_maps[MAP_SUPPORTED_QTY];	    // list of supported maps
    int m_mapnum[MAP_SUPPORTED_QTY];    // real map number (0 for 0 and 1, 2 for 2, and so on) - file name
    int m_mapid[MAP_SUPPORTED_QTY];	    // map id used by the client
    int m_sizex[MAP_SUPPORTED_QTY];
    int m_sizey[MAP_SUPPORTED_QTY];
    int m_sectorsize[MAP_SUPPORTED_QTY];

    bool m_mapsinitalized[MAP_SUPPORTED_QTY];

    // Pre-calculated values, for faster retrieval
    int _sectorcolumns[MAP_SUPPORTED_QTY];  // how much sectors are in a column (x) in a given map
    int _sectorrows[MAP_SUPPORTED_QTY];     // how much sectors are in a row (y) in a given map
    int _sectorqty[MAP_SUPPORTED_QTY];      // how much sectors are in a map

    CServerMapDiffCollection * m_pMapDiffCollection;

public:
    /** @name Constructors, Destructor, Asign operator:
     */
    ///@{
    CUOMapList();
private:
    CUOMapList(const CUOMapList& copy);
    CUOMapList& operator=(const CUOMapList& other);
    ///@}
public:
    /** @name Modifiers:
     */
    ///@{
    void Clear();
    bool ResetMap(int map, int maxx, int maxy, int sectorsize, int realmapnum, int mapid);
    void Init();
    bool Load(int map, char *args);
    ///@}
    /** @name Operations:
     */
    ///@{
protected:
    bool DetectMapSize(int map);
public:
    bool IsMapSupported(int map) const noexcept;
    bool IsInitialized(int map) const;
    int GetSectorSize(int map) const;
    int CalcSectorQty(int map) const;  // Use it only when initializing the map sectors! (because it's slower than the Get* method)
    int GetSectorQty(int map) const;
    int CalcSectorCols(int map) const; // Use it only when initializing the map sectors! (because it's slower than the Get* method)
    int CalcSectorRows(int map) const; // Use it only when initializing the map sectors! (because it's slower than the Get* method)
    int GetSectorCols(int map) const;
    int GetSectorRows(int map) const;
    int GetX(int map) const;
    int GetY(int map) const;
    int GetCenterX(int map) const;
    int GetCenterY(int map) const;

    int GetMapFileNum(int map) const;
    int GetMapID(int map) const;
    
    ///@}
} g_MapList;


// Turn off structure packing.
#if defined(_WIN32) && defined(_MSC_VER)
	#pragma pack()
#else
	#undef PACK_NEEDED
#endif

#endif //_INC_CUOMAPLIST_H
