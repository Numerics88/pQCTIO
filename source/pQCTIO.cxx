// Copyright (c) Numerics88
// See LICENSE for details.

#include "pQCTIO/pQCTIO.h"
#include "pQCTIO_Types.h"
#include <iostream>

namespace pQCTIO
{

void CtFile::ReadImageInfo() {
    // Open file.
    std::ifstream f (this->filename.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!f) {
      throw_pqctio_exception (std::string("Unable to open file ") + filename);
    }
    f.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    // Read header. If an exception occurs, be sure to close the file.
    try {
        this->ReadHeader (f);
    } catch (const pQCTIOException & e) {
        f.close();
        throw;
    }

    f.close();
}

void CtFile::ReadHeader(std::ifstream& f) {
    // Header starts at 0, duh
    f.seekg(0, f.beg);
    HeadPrefix_t header;

    // Read file
    f.read((char*)&header, sizeof(HeadPrefix_t));

    // Check if we successfully read in all the bytes
    if (f.gcount() != sizeof(HeadPrefix_t)) {
        throw_pqctio_exception (std::string("Cannot read header from file ") + filename);
    }

    // Test that it is a Ct file
    if (header.HeadLen != CT_HEADER_LENGTH) {
        throw_pqctio_exception (std::string("Currently, only CT is supported. File ") + filename + " is a scout view file.");
    }

    // Read in remaining data
    this->ComputeNumberOfVoxels(f);
    this->ReadHeaderData(f);
}

void CtFile::ComputeNumberOfVoxels(std::ifstream& f) {
    // Determine file size
    f.clear();                  // It's possible we reached EOF
    f.seekg(0, f.beg);
    long begin = f.tellg();
    f.seekg(0, f.end);
    long end = f.tellg();
    f.clear();                  // Definitely reached EOF
    f.seekg(0, f.beg);
    
    // Divide by two because voxel data is 16 bit
    this->number_of_voxels = (end-begin-CT_HEADER_LENGTH)/2;

    // Test that we have a valid number of voxels.
    if (this->number_of_voxels < 0) {
        throw_pqctio_exception ("Calculated negative voxels which isn't possible.");
    }
}

void CtFile::ReadHeaderData(std::ifstream& f) {
    // Seek to the beginning of the file, cautious against reaching the end.
    f.clear();
    f.seekg(0, f.beg);
    HeaderData_t headerData;

    // Read file
    f.read((char*)&headerData, sizeof(HeaderData_t));

    // Copy data from header to class
    this->VoxelSize   = headerData.VoxelSize;
    this->ObjLen      = headerData.ObjLen;
    this->MeasInfo    = std::string(headerData.MeasInfo);
    this->MeasDate    = headerData.MeasDate;
    this->Device      = std::string(headerData.Device);
    this->PatMeasNo   = headerData.PatMeasNo;
    this->PatNo       = headerData.PatNo;
    this->PatBirth    = headerData.PatBirth;
    this->PatName     = std::string(headerData.PatName);
    this->PatID       = std::string(headerData.PatID);
    this->PicX0       = headerData.PicX0;
    this->PicY0       = headerData.PicY0;
    this->PicMatrixX  = headerData.PicMatrixX;
    this->PicMatrixY  = headerData.PicMatrixY;

    // Test header data
    if (this->PicMatrixX * this->PicMatrixY != this->number_of_voxels) {
        throw_pqctio_exception ("Bad file. PicMatrixX*PicMatrixY does not equal number of voxels.");
    }
}

void CtFile::ReadImageData(short* data, size_t size) {
    // Open file.
    std::ifstream f (this->filename.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!f) {
      throw_pqctio_exception (std::string("Unable to open file ") + filename);
    }
    f.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    // Seek to where data begins
    f.seekg(CT_HEADER_LENGTH);
    f.read((char*)data, size);

    // Check if we successfully read in all the bytes
    if (f.gcount() != size) {
        f.close();
        throw_pqctio_exception (std::string("Could not read all image data from file ") + filename);
    }

    // Close file
    f.close();
}

CtFile::CtFile()
{}

CtFile::CtFile(const char* fn):
    filename (fn)
{}

CtFile::CtFile(const std::string fn):
    filename (fn.c_str())
{}

std::ostream& operator<<(std::ostream& out, CtFile const& obj)
{
    out << "Filename:           " << obj.filename << std::endl;
    out << "Number of Voxels:   " << obj.number_of_voxels << std::endl;
    out << "Header:             " << std::endl;
    out << "  VoxelSize:        " << obj.VoxelSize << std::endl;
    out << "  ObjLen:           " << obj.ObjLen << std::endl;
    out << "  PicX0:            " << obj.PicX0 << std::endl;
    out << "  PicY0:            " << obj.PicY0 << std::endl;
    out << "  PicMatrixX:       " << obj.PicMatrixX << std::endl;
    out << "  PicMatrixY:       " << obj.PicMatrixY << std::endl;
    out << "  PatNo:            " << obj.PatNo << std::endl;
    out << "  PatMeasNo:        " << obj.PatMeasNo << std::endl;
    out << "  PatBirth:         " << obj.PatBirth << std::endl;
    out << "  MeasDate:         " << obj.MeasDate << std::endl;
    out << "  PatName:          " << obj.PatName << std::endl;
    out << "  MeasInfo:         " << obj.MeasInfo << std::endl;
    out << "  Device:           " << obj.Device << std::endl;
    out << "  PatID:            " << obj.PatID << std::endl;

    return out;
}

} /* end namespace pQCTIO */
