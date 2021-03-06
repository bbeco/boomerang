#pragma region License
/*
 * This file is part of the Boomerang Decompiler.
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 */
#pragma endregion License
#pragma once


#include "boomerang/core/BoomerangAPI.h"


class UserProc;
class QString;


class BOOMERANG_API DFGWriter
{
public:
    void printDFG(const UserProc *proc, const QString &fname);
};
