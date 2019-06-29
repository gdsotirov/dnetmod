/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cid.cpp                   Type        : source            *
 *  Description : CIdentificator class implementation.                      *
 ****************************************************************************/

/**
 * @file cid.cpp
 * CIdentificator class implementation.
 */

#include <string.h>

#include "cid.h"

unsigned long CIdentificator::ulClassID = 101;
char CIdentificator::strClassName[] = "CIdentificator";

/** @brief Default constructor. Does nothing */
CIdentificator::CIdentificator() {}

/**
 * @brief Retrieves class identification name
 *
 * Retrieves class identification name (ClassName).
 * @remarks For the identification the static member #strClassName is used.
 * Thus member's value is the same for all objects of type CIdentificator and
 * it descendants. If you want to identify a descendant of CIdentificator
 * differently you must redeclare this member and initialize it like you want.
 * @param ulStrSz
 * @param strName
 */
void CIdentificator::GetClassName(unsigned long ulStrSz, char *strName) const {
    strncpy(strName, strClassName, ulStrSz);
}

/**
 * @brief Checks if a class can identify itself with a specified number
 *
 * This function can be used to check if a class or it ancestors can identify
 * themselves with the specified number. Thus you can check if the object
 * you use is from the type you want.
 * @remarks It is generally a good idea to override this function in your
 * descendants when you redeclare the class identification members ClassID.
 * A common way to implement this function is to compare the ID of your class
 * with the ID passed as parameter to the function and in case they don't
 * match to recurse invoking the base class function. Recursion will end in
 * base class CIdentificator::IsA function where only compare is done. All
 * classes from the module hierarchy have this behavior.
 * @param ulCompareID ID to be compared
 * @return True when match otherwise false
 */
bool CIdentificator::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID );
}

/**
 * @brief Checks if a class can identify itself with a specified name
 *
 * This function can be used to check if a class or it ancestors can identify
 * themselves with the specified name. Thus you can check if the object
 * you use is from the type you want.
 * @remarks It is generally a good idea to override this function in your
 * descendants when you redeclare the class identification member ClassName.
 * A common way to implement this function is to compare the Name of your class
 * with the Name passed as parameter to the function and in case they don't
 * match to recurse invoking the base class function. Recursion will end in
 * base class CIdentificator::IsA function where only compare is done. All
 * classes from the module hierarchy have this behavior.
 * @param strCompareName Name to be compared
 * @return True when match otherwise false
 */
bool CIdentificator::IsA(const char *strCompareName) const {
    return ( strcmp(strCompareName, strClassName) == 0 );
}

/** @brief Destructor. Does nothing */
CIdentificator::~CIdentificator() {}

