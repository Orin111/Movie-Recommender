#include "RSUsersLoader.h"
#include <fstream>
#include <istream>
#include <sstream>

#define FILE_ERROR "ERROR: could not open file"
#define NA "NA"
#define DASH '-'

/**
 * a func that push all users to the users vector
 * @param line a line in the text
 * @param tmp a "word"
 * @param users_vec vector of all users
 * @param movies_vec  vector of all movies
 * @param rs_shared shared pointer to recommender system
 */
void
users_vec_emplace_back (std::string &line, std::string tmp,
                        std::vector<RSUser> &users_vec,
                        std::vector<sp_movie> &movies_vec,
                        std::shared_ptr<RecommenderSystem> &rs_shared);

/**
 * a func that push all movies to the vector movies
 * @param tmp a "word"
 * @param movies_vec a vector of movies
 */
void movies_vec_push_back (const std::string &tmp,
                           std::vector<sp_movie> &movies_vec);

std::vector<RSUser> RSUsersLoader::create_users_from_file
    (const std::string &users_file_path, std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  std::shared_ptr<RecommenderSystem> rs_shared = std::move (rs);
  std::ifstream file (users_file_path);
  if (!file)
    {
      throw std::runtime_error (FILE_ERROR);
    }
  std::vector<RSUser> users_vec;
  std::vector<sp_movie> movies_vec;
  std::string line, movies;
  std::getline (file, movies);
  std::istringstream stream (movies);
  std::string tmp;
  while (stream >> tmp)
    {
      movies_vec_push_back (tmp, movies_vec);
    }
  while (std::getline (file, line))
    {
      users_vec_emplace_back (line, tmp, users_vec, movies_vec, rs_shared);
    }
  return users_vec;
}

void users_vec_emplace_back (std::string &line, std::string tmp,
                        std::vector<RSUser> &users_vec,
                        std::vector<sp_movie> &movies_vec,
                        std::shared_ptr<RecommenderSystem> &rs_shared)
{
  int counter = 0;
  std::istringstream stream2 (line);
  std::string name;
  stream2 >> name;
  rank_map rm (0, sp_movie_hash, sp_movie_equal);
  while (stream2 >> tmp)
    {
      double rank = 0;
      if (tmp != NA)
        {
          rank = std::stod (tmp);
        }
      rm.insert ({movies_vec[counter], rank});
      counter++;
    }
  users_vec.emplace_back (name, rm, rs_shared);
}

void movies_vec_push_back (const std::string &tmp,
                           std::vector<sp_movie> &movies_vec)
{
  int index1 = tmp.find (DASH);
  std::string movie_name = tmp.substr (0, index1);
  int movie_year = stoi (tmp.substr (index1 + 1));
  sp_movie movie = std::make_shared<Movie> (movie_name, movie_year);
  movies_vec.push_back (movie);
}