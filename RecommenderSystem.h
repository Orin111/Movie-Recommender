
#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>

typedef std::map<sp_movie, std::vector<double>, equal_func> map;

class RecommenderSystem
{
 private:
  map _map;
  /**
   *
   * @param vec
   * @param average
   * @return an normal vector (movie rank - average)
   */
  std::vector<double>
  pref_vec (std::vector<std::pair<sp_movie, double>> vec, double average);
  /**
   *
   * @param user
   * @param movie
   * @return a map of all movies and their similarities
   */
  std::multimap<double, sp_movie> movie_predict
      (const RSUser &user, const sp_movie &movie);

 public:
  RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based on movie
   * features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &
  operator<< (std::ostream &os, const RecommenderSystem &r);

};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
