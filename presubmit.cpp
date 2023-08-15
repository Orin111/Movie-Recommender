#include "RSUsersLoader.h"
#include "RecommenderSystemLoader.h"
#include <iostream>
#include <string>
#include <fstream>

static std::string BASE_PATH = "../";
#define THRESHOLD 0.01

int test_output ();

int main ()
{
  int status = 0;

  try
	{
	  test_output ();
	  // check all function for RS exist
	  RecommenderSystem rec;
	  sp_movie a = rec.add_movie ("Batman", 2022, { 1, 2, 3, 4 });
	  sp_movie b = rec.add_movie ("StarWars", 1977, { 1, 2, 3, 5 });
	  sp_movie c = rec.add_movie ("ForestGump", 1994, { 1, 2, 3, 4 });

	  // check all functions for user and UsersLoader exist and movie
	  auto rs1 = RecommenderSystemLoader::create_rs_from_movies_file (
		   "../presubmit.in_m");
	  std::vector<RSUser> users = RSUsersLoader::create_users_from_file (
		   "../presubmit.in_u", std::move (rs1));
      RSUser user = users[0];
	  sp_movie movie = user.get_recommendation_by_content ();
	  double s = users[0].get_prediction_score_for_movie ("Titanic", 1997, 2);
	  sp_movie m2 = users[0].get_recommendation_by_cf (2);
	  if (s < 0)
		{
		  std::cerr << "a score should be larger or equal to 0" << std::endl;
		  status = 1;
		}

	  // check movie functions and some results
	  if (movie->get_name () != "Batman" || movie->get_year () != 2022)
		{
		  status = 1;
		  std::cout
			  << "Recommend by content with username=Sofia failed! Received="
			  << *movie << " Expected=Batman (2022)" << std::endl;
		}
    double y = users[2].get_prediction_score_for_movie ("Twilight", 2008, 2);
	  if (std::abs (y - 3.5244) > THRESHOLD)
		{
		  status = 1;
		  std::cout
			  << "Predict movie score for moviename=Twilight , username=Nicole, k=2 failed! Received="
			  << y << " Expected=3.5244" << std::endl;
		}
	  double z = users[2].get_prediction_score_for_movie ("Titanic", 1997, 2);
	  if (std::abs (z - 5.46432) > THRESHOLD)
		{
		  status = 1;
		  std::cout
			  << "Predict movie score for moviename=Titanic, username=Nicole, k=2 failed! Received="
			  << z << " Expected=5.46432" << std::endl;
		}
	  if (status != 0)
		{
		  std::cerr << "Failed presubmit tests, you have errors" << std::endl;
		}
	  else
		{
		  std::cout << "Presubmit test passed" << std::endl;
		}
	}
  catch (const std::exception& e)
	{
	  std::cerr
		  << "Your program crashed by arised exception - presubmit tests failed"
		  << std::endl;
	  return 2;
	}
  if (status != 0)
	{
	  std::cerr << "you have failed the presubmit please check your errors"
				<< std::endl;
	  return 1;
	}
  return 0;
}

int compare_output ()
{
  std::ifstream in ("../test_out.txt"); //open two ins and one out
  std::ifstream in2 ("../print_main_out.txt");
  std::string line, line2;
  while ((!in.eof ()) && (!in2.eof ()))
	{ //continue you get til the end of both

	  getline (in, line); //get lines
	  getline (in2, line2);
	  if (line != line2)
		{
		  return 1;
		}
	}

  in.close ();
  in2.close ();   //close them

  return 0;  //open your out file and enjoy
}

int test_output ()
{
  std::ofstream out ("../test_out.txt");
  std::streambuf *coutbuf = std::cout.rdbuf (); //save old buf
  std::cout.rdbuf (out.rdbuf ()); //redirect std::cout to out.txt!

  Movie m ("A", 1999);
  std::cout << m << std::endl;
  auto rs = RecommenderSystemLoader::create_rs_from_movies_file ("RecommenderSystemLoader_input.txt");
  std::cout << *rs << std::endl;
  auto users = RSUsersLoader::create_users_from_file ("RSUsersLoader_input.txt", std::move (rs));
  std::cout << users[0] << std::endl;
  std::cout.rdbuf(coutbuf);
  if (compare_output () == 1)
	{
	  throw std::runtime_error ("Output don't match");
	}
}

