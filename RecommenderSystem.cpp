#include "RecommenderSystem.h"
#include <vector>
#include <cmath>

/**
 *
 * @param m1 movie1
 * @param m2 movie2
 * @return m1->year < m2->year
 */
bool equal_fanc (const sp_movie &m1, const sp_movie &m2);

/**
 *
 * @param user
 * @param sum the func will sum all movies rank
 * @return return a vector of movies and their ranks (only movies with rank>0)
 */
std::vector<std::pair<sp_movie, double>>
create_vec (const RSUser &user, double &sum);

/**
 *
 * @param vec1
 * @param vec2
 * @return the scalar mult of those 2 vectors
 */
double mult_vec (std::vector<double> vec1, std::vector<double> vec2);

/**
 *
 * @param pref_vec
 * @param features_vec
 * @return the similarity of those 2 vecturs calculate like this:
 * scalar mult of 2 vec / (norm of pref_vec * norm of features_vec)
 */
double vector_similarity (const std::vector<double> &pref_vec,
                          const std::vector<double> &features_vec);

bool equal_fanc (const sp_movie &m1, const sp_movie &m2)
{
  if (m1->get_year () != m2->get_year ())
    {
      return m1->get_year () < m2->get_year ();
    }
  return m1->get_name () < m2->get_name ();
}

RecommenderSystem::RecommenderSystem () : _map (equal_fanc)
{}

sp_movie RecommenderSystem::add_movie
    (const std::string &name, int year, const std::vector<double> &features)
{
  sp_movie sp = std::make_shared<Movie> (name, year);
  _map.insert ({sp, features});
  return sp;
}

std::vector<std::pair<sp_movie, double>>
create_vec (const RSUser &user, double &sum)
{
  auto vec = std::vector<std::pair<sp_movie, double>> ();
  for (const auto &i: user.get_rank ())
    {
      if (i.second != 0)
        {
          vec.push_back (i);
          sum += i.second;
        }
    }
  return vec;
}

std::vector<double>
RecommenderSystem::pref_vec (std::vector<std::pair<sp_movie, double>> vec,
                             double average)
{
  size_t size = _map[vec[0].first].size ();
  std::vector<double> res (size);
  for (const auto &i: vec)
    {
      for (int j = 0; j < static_cast<int>(size); ++j)
        {
          res[j] += ((i.second - average) * _map[i.first][j]);
        }
    }
  return res;
}

double mult_vec (std::vector<double> vec1, std::vector<double> vec2)
{
  double res = 0;
  size_t size = vec1.size ();
  for (int i = 0; i < static_cast<int>(size); ++i)
    {
      res += vec1[i] * vec2[i];
    }
  return res;
}

double vector_similarity (const std::vector<double> &pref_vec,
                          const std::vector<double> &features_vec)
{
  double mult = mult_vec (pref_vec, features_vec);
  double norm1 = sqrt (mult_vec (pref_vec, pref_vec));
  double norm2 = sqrt (mult_vec (features_vec, features_vec));
  return mult / (norm1 * norm2);
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  double sum = 0;
  auto vec = create_vec (user, sum);
  //calculate average
  double average = sum / (double) vec.size ();
  //normal vector (vec-average)
  std::vector<double> pref_vec1 = pref_vec (vec, average);
  double max_similarity_num = 0, cur_similarity = 0;
  sp_movie max_similarity_movie;
  for (const auto &i: user.get_rank ())
    {
      if (i.second == 0)
        {
          cur_similarity = vector_similarity (pref_vec1,
                                              _map[i.first]);
          if (cur_similarity > max_similarity_num)
            {
              max_similarity_num = cur_similarity;
              max_similarity_movie = i.first;
            }
        }
    }
  return max_similarity_movie;
}

std::multimap<double, sp_movie>
RecommenderSystem::movie_predict (const RSUser &user, const sp_movie &movie)
{
  auto res = std::multimap<double, sp_movie>{};
  for (const auto &i: user.get_rank ())
    {
      if (i.second != 0)
        {
          double cur = vector_similarity (_map[i.first],
                                          _map[movie]);
          res.insert (std::pair<double, sp_movie> (cur, i.first));
        }
    }
  return res;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user,
                                        const sp_movie &movie, int k)
{
  int counter = 0;
  double sum1 = 0, sum2 = 0;
  auto res = movie_predict (user, movie);
  auto map = user.get_rank ();

  for (auto i = res.rbegin (); i != res.rend (); i++)
    {
      if (counter < k)
        {
          sum1 += i->first * map[i->second];
          sum2 += i->first;
        }
      else
        {
          break;
        }
      counter++;
    }
  return sum1 / sum2;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie max_movie;
  double max_score = 0, cur = 0;
  for (const auto &i: user.get_rank ())
    {
      if (i.second == 0)
        {
          cur = predict_movie_score (user, i.first, k);
          if (cur > max_score)
            {
              max_score = cur;
              max_movie = i.first;
            }
        }
    }
  return max_movie;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  auto iter = _map.find (movie);
  if (iter == _map.end ())
    {
      movie = nullptr;
      return movie;
    }
  return iter->first;
}

std::ostream &operator<< (std::ostream &os, const RecommenderSystem &r)
{
  for (const auto &i: r._map)
    {
      os << (*i.first);
    }
  return os;
}

