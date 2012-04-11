#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>

int main (int argc, char *argv[])
{
  std::ifstream inFile;
  std::ofstream outFile;
  float Attribute, Variance, Max = 0.0;
  std::vector<float> v_Mean, v_Variance;
  char Line[40];
  unsigned int NbVertices;
  
  for (int FileNb = 1; FileNb < argc-1; FileNb++)
    {
      // Reading file
      std::cout<<"Reading file: "<<argv[FileNb]<<std::endl;
  
      inFile.open(argv[FileNb]);
      if (!inFile)
	{
	  std::cerr << "Unable to open file"<<std::endl;
	  exit(1);
	}

      while ( std::strncmp (Line, "NUMBER_OF_POINTS =", 18) && strncmp (Line, "NUMBER_OF_POINTS=", 17))
	inFile.getline (Line, 40);

      if (FileNb == 1 )
	{
	  NbVertices = atoi(strrchr(Line,'=')+1);
	  inFile.getline(Line, 40);
	  inFile.getline(Line, 40);      
	  
	  while(!inFile.eof())
	    {
	      inFile >> Attribute;
	      v_Mean.push_back(Attribute);
	      if (Attribute > Max)
		Max = Attribute;
	    }
	}
      else
	{
	  int NbPoints = atoi(strrchr(Line,'=')+1);
	  if (NbPoints != NbVertices)
	    {
	      std::cerr << "Attribute Files don't have the same number of vertices!"<<std::endl;
	      exit(1);
	    }
	  inFile.getline(Line, 40);
	  inFile.getline(Line, 40);      
	  
	  int Point = 0;
	  while(!inFile.eof())
	  {
	    inFile >> Attribute;
	    v_Mean[Point] += Attribute;
	    Point++;
	  }
	}
      inFile.close();
    }
  
  //Computing mean attribute
  std::cout<<"Computing mean attribute..."<<std::endl;
  for (unsigned int i = 0; i < NbVertices; i++)
    v_Mean[i] /= argc-2;

  //Computing Variance
  for (int FileNb = 1; FileNb < argc-1; FileNb++)
    {
      // Reading file
      std::cout<<"Reading file: "<<argv[FileNb]<<std::endl;
  
      inFile.open(argv[FileNb]);
      if (!inFile)
	{
	  std::cerr << "Unable to open file"<<std::endl;
	  exit(1);
	}

      while ( std::strncmp (Line, "NUMBER_OF_POINTS =", 18) && strncmp (Line, "NUMBER_OF_POINTS=", 17))
	inFile.getline (Line, 40);

      if (FileNb == 1 )
	{
	  NbVertices = atoi(strrchr(Line,'=')+1);
	  inFile.getline(Line, 40);
	  inFile.getline(Line, 40);      
	  
	  int Point = 0;
	  while(!inFile.eof())
	    {
	      inFile >> Attribute;
	      Variance = pow(Attribute-v_Mean[Point],2);
	      v_Variance.push_back(Variance);
	      Point++;
	    }
	}
      else
	{
	  int NbPoints = atoi(strrchr(Line,'=')+1);
	  if (NbPoints != NbVertices)
	    {
	      std::cerr << "Attribute Files don't have the same number of vertices!"<<std::endl;
	      exit(1);
	    }
	  inFile.getline(Line, 40);
	  inFile.getline(Line, 40);      
	  
	  int Point = 0;
	  while(!inFile.eof())
	  {
	    inFile >> Attribute;
	    Variance = pow(Attribute-v_Mean[Point],2);
	    v_Variance[Point] += Variance;
	    Point++;
	  }
	}
      inFile.close();
    }

  //Computing final variance
  std::cout<<"Computing final variance attribute..."<<std::endl;
  for (unsigned int i = 0; i < NbVertices; i++)
    {
      v_Variance[i] /= fabs(Max);
      v_Variance[i] /= argc-2;
    }

	
  // Writing output file
  std::cout<<"Writing output file..."<<std::endl;
  outFile.open(argv[argc-1]);
  if (!outFile)
    {
      std::cout << "Unable to open file";
      exit(1);
    }
  outFile<<"NUMBER_OF_POINTS="<<NbVertices<<std::endl;
  outFile<<"DIMENSION="<<1<<std::endl;
  outFile<<"TYPE=Scalar"<<std::endl;  
  for (unsigned int i = 0; i < NbVertices; i++)
    outFile<<v_Variance[i]<<std::endl;
  outFile.close();

  return EXIT_SUCCESS;
}
