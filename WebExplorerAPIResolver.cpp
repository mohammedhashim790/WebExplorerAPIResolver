// WebExplorerAPIResolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include<queue>
#include <filesystem>


using namespace std;


//Understanding Operator Overloading
//class Point {
//
//public:
//    Point(int theX, int theY) :x(theX), y(theY)
//    {}
//    // Print the object
//    friend ostream& operator <<(ostream& outputStream, const Point& p);
//private:
//    int x;
//    int y;
//};
//
//ostream& operator <<(ostream& outputStream, const Point& p) {
//    int posX = p.x;
//    int posY = p.y;
//
//    outputStream << "x=" << posX << "," << "y=" << posY;
//    return outputStream;
//}


DWORD mydrives = 100;
char lpBuffer[100];

namespace fs = std::filesystem;


string listLogicalDrives();

void ListEntitiesDFS(string fromPath);
string ListEntitiesBFS(string fromPath);
void SampleRecursive(string fromPath);

string accumulate(vector<string> vector);
bool isValidDirectory(fs::directory_entry entry);


class File {
private:
    string name;
    fs::directory_entry entryPoint;
    size_t fileSize;



public:
    void setName(string fileName) {
        this->name = fileName;
    }

    string getName() {
        if (this->name.empty()) {
            return "null";
        }
        return "\"" + this->name + "\""; 
    }

    void setFileSize(size_t fileSize) {
        this->fileSize = fileSize;
    }

    size_t getFileSize() {
        return fileSize;
    }



    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }

    string toString() {
        return "{\"Name\" : " + this->getName()+",\"Path\" : \"" + getEntryPoint().path().generic_string() + "\"" + "}";
    }


    File() {

    }
    File(string name) {
        this->setName(name);
    }


    File(string name, fs::directory_entry entryPoint) {
        this->setName(name);
        this->setEntryPoint(entryPoint);
        this->setFileSize(entryPoint.file_size());
    }

};

class Folder{


/// <summary>
///     Example
/// Folder folder;
/// folder.setName("*");
/// folder.addFile(File("12-03"));
/// folder.addFolder(Folder("Hello"));

/// folder.addFolder(Folder("New Folder"));
/// folder.addFile(File("File1"));


/// cout << folder.toString() << endl;
/// </summary>

private:
    string name;
    fs::directory_entry entryPoint;

    

    vector<Folder*> subFolders;

    vector<File*> subFiles;
    //vector<File*> subFiles;


    string accumulateFolders() {
        string res;
        for (Folder* folder : subFolders) {
            //res += "\"";
            res += folder->toString();;
            //res += "\",";
            res += ",";
        }
        //res += "}";
        if (res.empty()) {
            return "null";
        }
        res = "[" + res;
        res = string(res.begin(), res.end() - 1);
        res += "]";
        return res;
    }

    string accumulateFiles() {
        string res;
        for (File* file : subFiles) {
            //res += "\"";
            res += file->toString();
            //res += "\",";
            res += ",";
        }
        if (res.empty()) {
            return "null";
        }
        res = "[" + res;
        res = string(res.begin(), res.end() - 1);
        res += "]";
        return res;
    }


    


public:
    void setName(string name) {
        this->name = name;
    }

    string getName() {
        return this->name;
    }

    void setEntryPoint(fs::directory_entry entryPoint) {
        this->entryPoint = entryPoint;
    }

    fs::directory_entry getEntryPoint() {
        return this->entryPoint;
    }



    string toString() {
        return "{\"Name \" : \"" + this->name  + "\",\"Path\" : \"" + getEntryPoint().path().generic_string() + "\"," + "\"subFolders\" : " + this->accumulateFolders() + ",\"files\" : " + this->accumulateFiles() + "}";
    }

    void addFile(File* fileName) {
        this->subFiles.push_back(fileName);
    }

    void addFolder(Folder* folder) {
        this->subFolders.push_back(folder);
    }

    Folder() {

    }

    Folder(string name) {
        this->setName(name);
    }


    Folder(string name,fs::directory_entry entryPoint) {
        this->setName(name);
        this->setEntryPoint(entryPoint);
    }

};



int main()
{
    int now = time(NULL);



    const string dDrive("D:\\");
    const string cDrive("C:\\");

    ListEntitiesBFS(cDrive);


    cout << endl << "Time Taken to Execute DFS : " << (time(NULL) - now)<<"\n\n\n";
}


void ListEntitiesDFS(string fromPath) {

    

    for (const auto& entry : fs::directory_iterator(fromPath)){
        cout << entry.path() << endl;
        if (entry.is_directory()) {
            ListEntitiesDFS(entry.path().string());
        }
    }

    cout << "\nEnd of " << fromPath << endl;
}


/// <summary>
/// Similar to @ListEntitiesBFS()
/// </summary>
/// <param name="fromPath"></param>
void SampleRecursive(string fromPath) {

    for (const auto& entry : fs::recursive_directory_iterator(fromPath)){
        cout << entry.path() << endl;
    }

    cout << "\nEnd of " << fromPath << endl;
}



string EntitiesFromPath(string fromPath) {
    return ListEntitiesBFS(fromPath);
}



string ListEntitiesBFS(string fromPath) {

    cout << "From Path " << fromPath << endl << endl;

    queue<Folder*> que;
    int iter = 0;
    int files = 0;

    fs::directory_entry rootEntry = fs::directory_entry(fromPath);
    Folder rootFolder(rootEntry.path().generic_string(),rootEntry);


    for (const auto& entry : fs::directory_iterator(fromPath)) {
        /*cout << entry.path() << " \t"  << isValidDirectory(entry) << endl;*/
        if (isValidDirectory(entry)) {
            Folder* folder = new Folder(entry.path().filename().generic_string(), entry);
            rootFolder.addFolder(folder);
            
            que.push(folder);
        }
        else {
            File *file = new File(entry.path().filename().generic_string(), entry);
            rootFolder.addFile(file);
        }
        //printf("%d\b",iter++);
    }

    vector<string> folderVector;
    vector<string> filesVector;



    //cout << "Reading Children " << endl;


    //while (!que.empty()) {
    //    cout << "Detail : " << que.front()->toString() << endl;
    //    que.pop();
    //}


    //cout << "RootFolder: \n\n" << rootFolder.toString();

    while (!que.empty()) {
        try {
            Folder *parent = que.front();

            Folder *pRef = parent;

            //cout << "Reading From : " << parent->toString() << endl;

            que.pop();
            
            fs::path path = pRef->getEntryPoint().path();


            for (const auto& entry : fs::directory_iterator(path, fs::directory_options::skip_permission_denied)) {

                //cout << "Details : " << entry.path() << " IsDire : " << isValidDirectory(entry) << endl;

                if (isValidDirectory(entry)) {
                    Folder *folder = new Folder(entry.path().filename().generic_string(), entry);
                    que.push(folder);
                    
                    parent->addFolder(folder);
                    folderVector.push_back(entry.path().generic_string());
                }
                else {
                    files++;
                    File *file = new File(entry.path().filename().generic_string(), entry);
                    parent->addFile(file);
                    filesVector.push_back(entry.path().generic_string());
                }
                printf("%d\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", iter++);
            }
        }
        catch (exception e) {
            //cout << "\n\n\nError  : " << e.what() << "\n\n\n";
            /*cout << e.what();*/
        }
    }


    cout << "\n\n\n";

    cout << "Total Folders : " << iter << " " << folderVector.size() << endl;
    cout << "Total Files : " << files << " " << filesVector.size() << endl;

    string res = rootFolder.toString();
    cout << "Root Folder : " << res.length();

    cout << "\nEnd of " << fromPath << endl;

    return res;
}

string accumulate(vector<string> vect) {
    string res("[");

    for (const auto& str : vect) {
        res += "\"";
        res += str;
        res += "\",";
    }

    return string(res.begin(),res.end()-1) + "]";
}

bool isValidDirectory(fs::directory_entry entry) {
    return
        (
            entry.is_directory() && 
            !(entry.path().generic_string().find("System Volume Information") != string::npos ||
            entry.path().generic_string().find("$RECYCLE.BIN") != string::npos));
}



/// <summary>
/// Return List of Drive Characters
/// </summary>
/// <returns>string</returns>
string listLogicalDrives() {

    /// <summary>
    /// Returns the BIT representation denoting Alphabets from LSB A-Z
    /// </summary>
    /// <returns> Integer </returns>
    DWORD bit = GetLogicalDrives();

    char drive = 'a';

    int buffer = 1;

    string drives = "[";

    for (drive = 'a'; drive <= 'z'; drive++) {
        if ((bit & buffer) != 0) {
            drives += "\"";
            drives += drive;
            drives += "\",";
        }
        buffer = buffer << 1;
    }
    drives = string(drives.begin(), drives.end() - 1);
    drives += ']';
    return drives;

}