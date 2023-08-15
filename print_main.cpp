//
// Created by 24565 on 6/1/2022.
//

#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"

int main ()
{
  Movie m ("A", 1999);
  std::cout << m << std::endl;
  auto rs = RecommenderSystemLoader::create_rs_from_movies_file ("RecommenderSystemLoader_input.txt");
  std::cout << *rs << std::endl;
  auto users = RSUsersLoader::create_users_from_file ("RSUsersLoader_input.txt", std::move (rs));
  std::cout << users[0] << std::endl;
  std::cout << users[2] << std::endl;
  std::cout << users[2].get_prediction_score_for_movie ("Twilight", 2008,2) << std::endl;
}