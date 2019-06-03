/* src/common/file.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#ifndef _SECPASS_COMMON_FILE_H
#define _SECPASS_COMMON_FILE_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class File {
  public:
    bool     is_open = false;
    string   path;
    ifstream fs;
    vector<string> names;

    File  (string filepath);
    ~File ();

    void open  ();
    void save  ();
    void close ();
};
#endif //_SECPASS_COMMON_FILE_H
