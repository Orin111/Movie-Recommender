#include "Movie.h"
#include "RSUser.h"
#include "RecommenderSystem.h"
#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"

int main ()
{

  sp_movie movie1 = std::make_shared<Movie> ("Titanic", 1997);
  sp_movie movie2 = std::make_shared<Movie> ("Twilight", 2008);
  sp_movie movie3 = std::make_shared<Movie> ("ForestGump", 1994);
  sp_movie movie4 = std::make_shared<Movie> ("Batman", 2022);
  sp_movie movie5 = std::make_shared<Movie> ("StarWars", 1977);

  rank_map r_map (0, sp_movie_hash, sp_movie_equal);
  r_map.insert (std::make_pair (movie1, 4));
  r_map.insert (std::make_pair (movie2, 0));
  r_map.insert (std::make_pair (movie3, 8));
  r_map.insert (std::make_pair (movie4, 0));
  r_map.insert (std::make_pair (movie5, 0));

  std::vector<double> m1f = {7, 2, 9, 1};
  std::vector<double> m2f = {3, 4, 6, 5};
  std::vector<double> m3f = {1, 7, 7, 6};
  std::vector<double> m4f = {2, 6, 4, 8};
  std::vector<double> m5f = {3, 3, 4, 9};

  std::shared_ptr<RecommenderSystem> rec_sys =
      std::make_shared<RecommenderSystem> ();

  rec_sys->add_movie (movie1->get_name (), movie1->get_year (), m1f);
  rec_sys->add_movie (movie2->get_name (), movie2->get_year (), m2f);
  rec_sys->add_movie (movie3->get_name (), movie3->get_year (), m3f);
  rec_sys->add_movie (movie4->get_name (), movie4->get_year (), m4f);
  rec_sys->add_movie (movie5->get_name (), movie5->get_year (), m5f);

  std::string name1 = "liel";
  std::string name2 = "idan";
  RSUser user1 (name1, r_map, rec_sys);
  RSUser user2 (name2, r_map, rec_sys);
  std::vector<double> m6f = {1, 10, 1, 10};
  user1.add_movie_to_rs ("liel", 1997, m6f, 9);
  std::cout << user1;
  std::cout << *(user1.get_recommendation_by_content ());
  std::cout << *(user1.get_recommendation_by_cf (2));

  std::unique_ptr<RecommenderSystem> rec_sys_2 =
      RecommenderSystemLoader::create_rs_from_movies_file
          ("RecommenderSystemLoader_input.txt");

  sp_movie found = (*rec_sys_2).get_movie ("invalid", 1234);
  std::cout << *rec_sys_2 << std::endl;
  std::vector<double> test_vec = {1, 1, 1, 1};
  rec_sys_2->add_movie ("test__movie", 1111, test_vec);
  std::cout << *rec_sys_2;

  //std::vector<RSUser> users = RSUsersLoader::create_users_from_file
  //  ("RSUsersLoader_input.txt",std::move(rec_sys_2));




  return 0;
}
