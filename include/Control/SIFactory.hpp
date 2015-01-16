#ifndef SIFACTORY_HPP
#define SIFACTORY_HPP

#include "stdafx.hpp"

class Intelligence;

class SIFactory
{
public:
    SIFactory();
    virtual ~SIFactory();



protected:
private:
    std::vector<std::tr1::shared_ptr<Intelligence> > m_spSIList;
};

#endif // SIFACTORY_HPP
