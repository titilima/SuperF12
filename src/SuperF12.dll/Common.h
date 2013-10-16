///////////////////////////////////////////////////////////////////////////////
// FileName:    Common.h
// Created:     2013/07/15
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: 预编译头公共文件
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_H
#define COMMON_H

#pragma once

/**
 * ATL & WTL Headers
 */

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlcrack.h>

/**
 * CRT & STL Headers
 */

#include <map>
#include <memory>

using namespace std;

/**
 * Local Headers
 */

#include "..\include\SuperF12.h"
#include "Globals.h"
#include "resource.h"

#if defined(_MSC_VER) || defined(__clang__)
#define OVERRIDE override
#else
#define OVERRIDE
#endif

#endif // COMMON_H
