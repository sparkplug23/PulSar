#pragma once


/**
 * @brief Test method for using to hold palettes
 * 
 * mPalette (or perhaps to be renamed) will hold all the loaded information and still be a singleton.
 * Maybe "mPaletteController"
 * 
 * 
 * This class, should use mPalette as a friend class (access via pointer only) and hold the loaded data of the active palette.
 * That way, each new palette I need (ie for each segment) will simply use another of this class and a vector for the data
 */


// class mPaletteLoad
// {
// private:
//     /* data */
// public:
//     mPaletteLoad(/* args */);
//     ~mPaletteLoad();
// };

// mPaletteLoad::mPaletteLoad(/* args */)
// {
// }

// mPaletteLoad::~mPaletteLoad()
// {
// }




