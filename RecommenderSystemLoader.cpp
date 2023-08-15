#include "RecommenderSystemLoader.h"
#include "RecommenderSystem.h"
#include <fstream>
#include <istream>
#include <sstream>

#define MIN 1
#define MAX 10
#define FILE_ERROR "ERROR: could not open file"

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  auto rs = std::make_unique<RecommenderSystem> ();
  std::ifstream file (movies_file_path);
  if (!file)
    {
      throw std::runtime_error (FILE_ERROR);
    }
  std::string line;
  while (std::getline (file, line))
    {
      std::istringstream stream (line);
      std::string movie_name, tmp;
      int movie_year, rank;
      std::vector<double> features;
      stream >> tmp;
      int index1 = tmp.find ('-');
      movie_name = tmp.substr (0, index1);
      movie_year = stoi (tmp.substr (index1 + MIN));
      while (stream >> rank && (MIN <= rank && rank <= MAX))
        {
          features.push_back (rank);
        }
      rs->add_movie (movie_name, movie_year, features);
    }
  return rs;
}