//=============================================================================
// File:       message.cpp
// Contents:   Definitions for DwMessage
// Maintainer: Doug Sauder <dwsauder@fwb.gulf.net>
// WWW:        http://www.fwb.gulf.net/~dwsauder/mimepp.html
// $Revision$
// $Date$
//
// Copyright (c) 1996, 1997 Douglas W. Sauder
// All rights reserved.
// 
// IN NO EVENT SHALL DOUGLAS W. SAUDER BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
// THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF DOUGLAS W. SAUDER
// HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// DOUGLAS W. SAUDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
// BASIS, AND DOUGLAS W. SAUDER HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
//=============================================================================

#define DW_IMPLEMENTATION

#include <mimelib/config.h>
#include <mimelib/debug.h>
#include <mimelib/string.h>
#include <mimelib/message.h>
#include <mimelib/header.h>
#include <mimelib/body.h>


const char* const DwMessage::sClassName = "DwMessage";


DwMessage* (*DwMessage::sNewMessage)(const DwString&,
    DwMessageComponent*) = 0;


DwMessage* DwMessage::NewMessage(const DwString& aStr,
    DwMessageComponent* aParent)
{
    if (sNewMessage) {
        return sNewMessage(aStr, aParent);
    }
    else {
        return new DwMessage(aStr, aParent);
    }
}


DwMessage::DwMessage()
{
    mClassId = eMessage;
    mClassName = sClassName;
}


DwMessage::DwMessage(const DwMessage& aMessage)
  : DwEntity(aMessage)
{
    mClassId = eMessage;
    mClassName = sClassName;
}


DwMessage::DwMessage(const DwString& aStr, DwMessageComponent* aParent)
  : DwEntity(aStr, aParent)
{
    mClassId = eMessage;
    mClassName = sClassName;
}


DwMessage::~DwMessage()
{
}


DwMessageComponent* DwMessage::Clone() const
{
    return new DwMessage(*this);
}


const DwMessage& DwMessage::operator = (const DwMessage& aMessage)
{
    if (this == &aMessage) return *this;
    DwEntity::operator = (aMessage);
    return *this;
}


void DwMessage::PrintDebugInfo(ostream& aStrm, int aDepth) const
{
#if defined(DW_DEBUG_VERSION)
    aStrm << "------------ Debug info for DwMessage class ------------\n";
    _PrintDebugInfo(aStrm);
    int depth = aDepth - 1;
    depth = (depth >= 0) ? depth : 0;
    if (aDepth == 0 || depth > 0) {
        mHeader->PrintDebugInfo(aStrm, depth);
        mBody->PrintDebugInfo(aStrm, depth);
    }
#endif // defined(DW_DEBUG_VERSION)
}


void DwMessage::_PrintDebugInfo(ostream& aStrm) const
{
#if defined(DW_DEBUG_VERSION)
    DwEntity::_PrintDebugInfo(aStrm);
#endif // defined(DW_DEBUG_VERSION)
}

