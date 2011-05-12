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

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include <boost/property_tree/ptree.hpp>

#include <libpc/XMLSchema.hpp>


#include <libpc/drivers/faux/Reader.hpp>
#include <libpc/drivers/faux/Writer.hpp>

#include <libpc/Iterator.hpp>
#include <libpc/Utils.hpp>

#include "Support.hpp"
#include "TestConfig.hpp"

#include <fstream>
using namespace libpc;




std::string ReadXML(std::string filename)
{

    std::istream* infile = Utils::openFile(filename, true);
    std::ifstream::pos_type size;
    // char* data;
    std::vector<char> data;
    if (infile->good()){
        infile->seekg(0, std::ios::end);
        size = infile->tellg();
        data.resize(static_cast<std::vector<char>::size_type>(size));
        // data = new char [size];
        infile->seekg (0, std::ios::beg);
        infile->read (&data.front(), size);
        // infile->close();

        // delete[] data;
        delete infile;
        return std::string(&data[0], data.size());
        // return data; 
    } 
    else 
    {   
        throw libpc_error("unable to open file!");
        // return data;
    }
    
}


BOOST_AUTO_TEST_SUITE(XMLSchemaTest)




BOOST_AUTO_TEST_CASE(test_schema)
{
    std::string xml = ReadXML(TestConfig::g_data_path+"schemas/8-dimension-schema.xml");
    std::string xsd = ReadXML(TestConfig::g_data_path+"/schemas/LAS.xsd");
    
    libpc::XMLSchema schema(xml, xsd);
    
    
}

// BOOST_AUTO_TEST_CASE(clean_up)
// {
//     if (!ShouldRunTest()) return;
//     
//     libpc::drivers::oci::Options options = GetOptions();
//     
//     Connection connection = Connect(options);
// 
//     std::string base_table_name = options.GetPTree().get<std::string>("base_table_name");
//     std::string block_table_name = options.GetPTree().get<std::string>("block_table_name");
//     
//     std::string drop_base_table = "DROP TABLE " + base_table_name;
//     std::string drop_block_table = "DROP TABLE " + block_table_name;
//     RunSQL(connection, drop_base_table);
//     RunSQL(connection, drop_block_table);    
// }

BOOST_AUTO_TEST_SUITE_END()