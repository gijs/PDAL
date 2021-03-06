/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/pdal_internal.hpp>
#ifdef PDAL_HAVE_PYTHON

#include <pdal/plang/Script.hpp>
#include <pdal/FileUtils.hpp>

#ifdef PDAL_COMPILER_MSVC
#  pragma warning(disable: 4127) // conditional expression is constant
#endif


namespace pdal { namespace plang {



static char* copychars(const char* s)
{
    char* p = new char[strlen(s)+1];
    strcpy(p, s);
    return p;
}


Script::Script(const std::string& source, const std::string& module, const std::string& function)
{
    m_source = copychars(source.c_str());
    m_module = copychars(module.c_str());
    m_function = copychars(function.c_str());
    return;
}


Script::Script(const Options& options)
{
    if (options.hasOption("source"))
    {
        const std::string& source = options.getValueOrThrow<std::string>("source");
        m_source = copychars(source.c_str());
    }
    else 
    {
        const std::string& filename = options.getValueOrThrow<std::string>("filename");
        std::string source = FileUtils::readFileIntoString(filename);
        m_source = copychars(source.c_str());
    }

    const std::string& module = options.getValueOrThrow<std::string>("module");
    const std::string& function = options.getValueOrThrow<std::string>("function");

    m_module = copychars(module.c_str());
    m_function = copychars(function.c_str());

    return;
}


Script::Script(const Script& script)
{
    m_source = copychars(script.source());
    m_module = copychars(script.module());
    m_function = copychars(script.function());
}


Script::~Script()
{
    delete m_source;
    delete m_module;
    delete m_function;
}

std::ostream& operator<<(std::ostream& os, Script const& script)
{
    int len = strlen(script.source());

    os << "source=[" << len << " bytes], ";
    os << "module=" << script.module() << ", ";
    os << "function=" << script.function();
    os << std::endl;

    return os;
}

} } //namespaces

#endif
