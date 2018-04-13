/*////////////////////////////////////////////////////////////////////////////
 Copyright 2018 Istituto Nazionale di Fisica Nucleare

 Licensed under the EUPL, Version 1.2 or - as soon they will be approved by
 the European Commission - subsequent versions of the EUPL (the "Licence").
 You may not use this work except in compliance with the Licence.

 You may obtain a copy of the Licence at:

 https://joinup.ec.europa.eu/software/page/eupl

 Unless required by applicable law or agreed to in writing, software
 distributed under the Licence is distributed on an "AS IS" basis, WITHOUT
 WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 Licence for the specific language governing permissions and limitations under
 the Licence.
////////////////////////////////////////////////////////////////////////////*/


#ifndef IBABSTRACTREADER_H
#define IBABSTRACTREADER_H

#include <Core/Object.h>

template <typename T>
class IBAbstractReader : public uLib::Object
{
    typedef uLib::Object BaseClass;
public:
    virtual ~IBAbstractReader() {}

    virtual unsigned long GetNumberOfEvents() =0;
    virtual T *GetNext() =0;

protected:
    IBAbstractReader() {}
    IBAbstractReader(IBAbstractReader &copy) {}
};










#endif // IBABSTRACTREADER_H
