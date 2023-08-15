//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"

// implement your cpp code here
RSUser::RSUser (std::string &user_name, rank_map _rank_map,
                std::shared_ptr<RecommenderSystem> _recom_sys)
    :
    _user_name (user_name), _rank_map (std::move (_rank_map)),
    _recom_sys (std::move (_recom_sys))
{}

const std::string &RSUser::get_name () const
{
  return _user_name;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features,
                              double rate)
{
  _rank_map.insert (std::make_pair (_recom_sys->add_movie
      (name, year, features), rate));
}

const rank_map &RSUser::get_rank () const
{
  return _rank_map;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _recom_sys->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _recom_sys->recommend_by_cf (*this, k);
}
double
RSUser::get_prediction_score_for_movie
    (const std::string &name, int year, int k) const
{
  return _recom_sys->predict_movie_score
      (*this, _recom_sys->get_movie (name, year), k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user.get_name () << std::endl;
  os << *user._recom_sys << std::endl;
  return os;
}










