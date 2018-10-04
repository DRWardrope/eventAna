// 
// Usage:
// root -l -b -q 'sumOfWeights.C+( <path> )'
//


// STL includes
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

// ROOT includes
#include "TFile.h"
#include "TH1F.h"



void read_directory(const std::string & directory_name, std::vector<string> & list, const std::string & pattern)
{
  list.clear();
  DIR * dirp = opendir(directory_name.c_str());
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    const std::string name = dp->d_name;
    if (name.find(pattern) != std::string::npos) {
      list.push_back(name);
    }
  }
  closedir(dirp);
}


std::string getcwd_string() 
{
  char buff[PATH_MAX];
  getcwd( buff, PATH_MAX );
  std::string cwd( buff );
  return cwd;
}


void sumOfWeights(const std::string & directory)
{
  
  std::ofstream outfile;
  outfile.open("sumOfWeights.txt");
  outfile << "# " << getcwd_string() << "/" << directory << "\n";
  outfile << "# NAME EVENTS SUMOFWEIGHTS \n";

  std::vector<std::string> listOfDirs;
  read_directory(directory, listOfDirs, "mc15_13TeV");

  for (unsigned int idir = 0; idir < listOfDirs.size(); ++idir) {
    
    std::string sub_directory = directory + "/" + listOfDirs.at(idir);
    std::vector<std::string> listOfFiles;
    read_directory(sub_directory, listOfFiles, ".root");
    
    float weights_total = 0.;
    float events_total = 0.;
    
    for (unsigned int ifile = 0; ifile < listOfFiles.size(); ++ifile) {

      std::string filename = listOfFiles.at(ifile);
      std::string path = sub_directory + "/" + filename;
      
      TFile * f = new TFile(path.c_str(), "read");
      if ( ! f->IsOpen() ) {
 	std::cout << "Error: couldn't open file \"" << path << "\"" << std::endl;
	continue;
      }
      
      TH1F * h = static_cast<TH1F *>(f->Get("MetaData_EventCount"));
      if ( ! h ) {
	std::cout << "Error: couldn't open histogram \"MetaData_EventCount\" in file \"" << path << "\"" << std::endl;
	continue;	
      }

      events_total  += h->GetBinContent(1);
      weights_total += h->GetBinContent(4);

      delete h;
      delete f;

    }

    outfile << listOfDirs.at(idir) << " " << events_total << " " << weights_total << "\n";

  }
  
  outfile.close();

}
