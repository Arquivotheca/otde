//=============================================================================
// File:       addrlist.h
// Contents:   Declarations for DwAddressList
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

#ifndef DW_ADDRLIST_H
#define DW_ADDRLIST_H

#ifndef DW_CONFIG_H
#include <mimelib/config.h>
#endif


//=============================================================================
//+ Name DwAddressList -- Class for containing DwAddress objects
//+ Description
//. {\tt DwAddressList} represents a list of {\it addresses} as described
//. in RFC-822.  As a C++ class, {\tt DwAddressList} is a container for
//. objects of type {\tt DwAddress}, and contains various member functions
//. to manage its contained objects.  {\tt DwAddressList} is also a
//. {\tt DwFieldBody}.  This reflects the fact that certain RFC-822 header
//. fields, such as the ``To:'' header field, have a list of
//. {\it addresses} as their {\it field-bodies}.
//=============================================================================


class DW_EXPORT DwAddressList : public DwFieldBody {

public:

    DwAddressList();
    DwAddressList(const DwAddressList& aList);
    DwAddressList(const DwString& aStr, DwMessageComponent* aParent=0);
    //. The first constructor is the default constructor, which sets the
    //. {\tt DwAddressList} object's string representation to the empty string
    //. and sets its parent to NULL.
    //.
    //. The second constructor is the copy constructor, which copies the
    //. string representation and all {\tt DwAddress}es from {\tt aList}.
    //. The parent of the new {\tt DwAddressList} object is set to NULL.
    //.
    //. The third constructor copies {\tt aStr} to the {\tt DwAddressList}
    //. object's string representation and sets {\tt aParent} as its parent.
    //. The virtual member function {\tt Parse()} should be called immediately
    //. after this constructor in order to parse the string representation.
    //. Unless it is NULL, {\tt aParent} should point to an object of a class
    //. derived from {\tt DwField}.

    virtual ~DwAddressList();

    const DwAddressList& operator = (const DwAddressList& aList);
    //. This is the assignment operator, which follows regular semantics.

    virtual void Parse();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the parse method for {\tt DwAddressList} objects.
    //. It should be called immediately after the string representation
    //. is modified and before the parts of the broken-down
    //. representation are accessed.

    virtual void Assemble();
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. executes the assemble method for {\tt DwAddressList} objects.
    //. It should be called whenever one of the object's attributes
    //. is changed in order to assemble the string representation from
    //. its broken-down representation.  It will be called
    //. automatically for this object by the parent object's
    //. {\tt Assemble()} member function if the is-modified flag is set.

    virtual DwMessageComponent* Clone() const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. creates a new {\tt DwAddressList} on the free store that has the same
    //. value as this {\tt DwAddressList} object.  The basic idea is that of
    //. a ``virtual copy constructor.''

    DwAddress* FirstAddress() const;
    //. Gets the first {\tt DwAddress} object in the list.  
    //. Use the member function {\tt DwAddress::Next()} to iterate.  
    //. Returns NULL if the list is empty.

    void Add(DwAddress* aAddr);
    //. Adds {\tt aAddr} to the end of the list of {\tt DwAddress} objects
    //. maintained by this {\tt DwAddressList} object.

    void Remove(DwAddress* aAddr);
    //. Removes {\tt aAddr} from the list of {\tt DwAddress} objects
    //. maintained by this {\tt DwAddressList} object.  The {\tt DwAddress}
    //. object is not deleted by this member function.

    void DeleteAll();
    //. Removes and deletes all {\tt DwAddress} objects from the list
    //. maintained by this {\tt DwAddressList} object.

    static DwAddressList* NewAddressList(const DwString& aStr,
        DwMessageComponent* aParent);
    //. Creates a new {\tt DwAddressList} object on the free store.
    //. If the static data member {\tt sNewAddressList} is NULL, 
    //. this member function will create a new {\tt DwAddressList}
    //. and return it.  Otherwise, {\tt NewAddressList()} will call
    //. the user-supplied function pointed to by {\tt sNewAddressList},
    //. which is assumed to return an object from a class derived from
    //. {\tt DwAddressList}, and return that object.

    //+ Var sNewAddressList
    static DwAddressList* (*sNewAddressList)(const DwString&,
        DwMessageComponent*);
    //. If {\tt sNewAddressList} is not NULL, it is assumed to point to a 
    //. user-supplied function that returns a pointer to an object
    //. from a class derived from {\tt DwAddressList}.

protected:

    DwAddress* mFirstAddress;
    //. Points to first {\tt DwMailbox} object in list.

private:

    static const char* const sClassName;

    void CopyList(const DwAddress* aFirstAddr);

public:

    virtual void PrintDebugInfo(ostream& aStrm, int aDepth=0) const;
    //. This virtual function, inherited from {\tt DwMessageComponent},
    //. prints debugging information about this object to {\tt aStrm}.
    //. It will also call {\tt PrintDebugInfo()} for any of its child
    //. components down to a level of {\tt aDepth}.
    //.
    //. This member function is available only in the debug version of
    //. the library.

    virtual void CheckInvariants() const;
    //. Aborts if one of the invariants of the object fails.  Use this
    //. member function to track down bugs.
    //.
    //. This member function is available only in the debug version of
    //. the library.

protected:

    void _PrintDebugInfo(ostream& aStrm) const;

};


class DW_EXPORT DwAddressListParser {
public:
    enum {
        eAddrError,
        eAddrGroup,
        eAddrMailbox,
        eAddrNull,
        eAddrEnd
    };
    DwAddressListParser(const DwString& aStr);
    virtual ~DwAddressListParser();
    const DwString& AddrString() { return mAddrString.Tokens(); }
    int AddrType()  { return mAddrType; }
    int IsGroup()   { return (mAddrType == eAddrGroup) ? 1 : 0; }
    int IsMailbox() { return (mAddrType == eAddrMailbox) ? 1 : 0; }
    int IsNull()    { return (mAddrType == eAddrNull) ? 1 : 0; }
    int IsEnd()     { return (mAddrType == eAddrEnd) ? 1 : 0; }
    int Restart();
    int operator ++ (); // prefix increment operator
protected:
    void ParseNextAddress();
    DwRfc822Tokenizer mTokenizer;
    DwTokenString mAddrString;
    int mAddrType;
private:
    DwAddressListParser();
    DwAddressListParser(const DwAddressListParser&);
    const DwAddressListParser& operator = (const DwAddressListParser&);
};

#endif
