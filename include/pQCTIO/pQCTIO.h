// Copyright (c) 2010-2025, Numerics88 Solutions.
// See LICENSE for details.

#ifndef __qPQCTIO_pQCTIO_h
#define __qPQCTIO_pQCTIO_h

#include "pqctio_export.h"
#include "pQCTIO/Exception.h"
#include <fstream>
#include <string>

namespace pQCTIO
{

/** Defines the pQCTFile.
 * 
 */
class PQCTIO_EXPORT pQCTFile {
public:
    /// Constructors.
    pQCTFile ();
    pQCTFile (const char* filename);
    pQCTFile (const std::string filename);

    /// Read all necessary information from the file.
    /// This function will throw a pqctio_exception if the
    /// file cannot be read. When this function is called,
    /// the following occurs:
    ///     1) Open the file as an input, binary file
    ///     2) Read the first 8 bytes from the header
    ///         2a) Verify that it is a CT file (as opposed to a Sv file)
    ///     3) Compute the number of voxels in the file
    ///         2a) Verify that number_of_voxels >= 0
    ///     4) Read remaining header data
    ///         4a) Test that PicMatrixX*PicMatrixY == number_of_voxels
    void ReadImageInfo();

    /// Read the image data 
    /// This should be performed after ReadImageInfo. See README.md for
    /// for examples of how to call this function. The user is responsible
    /// for allocating the data before calling this function.
    void ReadImageData(short* data, size_t size);

    /// Helper variables
    long                    number_of_voxels;
    std::string             filename;

    /// Header variables
    double      VoxelSize;
    double      ObjLen;
    int         PicX0;
    int         PicY0;
    int         PicMatrixX;
    int         PicMatrixY;
    long        PatNo;
    int         PatMeasNo;
    long        PatBirth;
    long        MeasDate;
    std::string PatName;
    std::string MeasInfo;
    std::string Device;
    std::string PatID;

    /// Helper function for printing the object.
    friend std::ostream& operator<<(std::ostream& out, pQCTFile const& obj);

private:
    /// Utility function for reading the header.
    /// At this point, the file stream should be valid.
    void ReadHeader(std::ifstream& f);

    /// Utility function for computing the number of voxels.
    /// At this point, the file stream should be valid. We determine the 
    /// size of the file by calculating the number of bytes we traverse 
    /// from start to finish. Subtract the header size, divide by 2 for 16 
    /// bit data and you get the number of voxels!
    void ComputeNumberOfVoxels(std::ifstream& f);

    /// Utility function for reading in the remaining header data.
    /// At this point, the file stream should be valid and the number
    /// of voxels should be computed. The remaining header data will be 
    /// read in and parsed into the class. Sadly, we were not given a
    /// fully defined header for the pQCT Ct files. As such, not every
    /// header element can be read.
    void ReadHeaderData(std::ifstream& f);
}; /* end class PQCTIO_EXPORT pQCTFile */

} /* end namepsace pQCTIO */

#endif /* __qPQCTIO_pQCTIO_h */
