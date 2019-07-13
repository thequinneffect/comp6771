/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

  - i definitely learnt as I went along which types of tests are more valuable. I don't really want
  to have to clean up all the tests are the start because of how long this file is 
  (and they are all still valid tests, some just probably aren't "necessary") and so i'd suggest 
  looking at test from friend operators onwards (in terms of marking testing style).

  - a lot of these tests are probably considered superfluous or a bit over the top and cautious,
  the reason/justification I give for them is that i'm simply not comfortable with all the 
  different ways things like construction, copying and moving can be performed (both in terms 
  of how the code is written and how C++ does things under the hood)
  TLDR: sorry the tests are so long and a bit superfluous in some cases but it was for my
  peace of mind

  - say there are x different possible cases to test in your system. Then in an ideal world
  you would only write x tests. However, some edge cases are hard to think of, and sometimes
  it's not easy to see if a test is meaningfull or not. Given this, my basic approach to testing
  for this assignment (and because i'm very new to lots of these c++ concepts e.g. operator overloading, 
  different constructor types etc.) has been to try and test as many different cases as I can. I've tried
  to only include tests that are doing something fundamentally different to all other tests but 
  i've allowed for possible meaningless tests as i'm more concerned with covering the x cases.

  - justification : for testing copy assignment operator= overload, I do not explicitly test that
  the argument EV "const EuclideanVector& original" (i.e. the one being copied from) is unchanged
  as the const keyword should enforce this to be the case.  



*/

#include "assignments/ev/euclidean_vector.h"

#include <iterator>
#include <list>
#include <vector>

#include "catch.h"

/* ctor testing */

SCENARIO("the {dimension, magnitude} constructor should validly construct EV's") {
  GIVEN("the {dimension, magnitude} constructor") {
    WHEN("we pass {0, 0.0} i.e. no dimension and consequently superfluous 0 magnitude") {
      EuclideanVector ev {0, 0.0};
      THEN("we should have a 0 dimension vector (and no magnitudes exist)") {
        REQUIRE(ev.GetNumDimensions() == 0);
      }
    }
    WHEN("we pass {0, 1.0} i.e. superfluous but non-zero 1.0 magnitude") {
      EuclideanVector ev {0, 1.0};
      THEN("we should have a 0 dimension vector (and no magnitudes exist)") {
        REQUIRE(ev.GetNumDimensions() == 0);
      }
    }
    WHEN("we pass {1, 0.0} i.e. now have non-zero dimensions and thus a used magnitude") {
      EuclideanVector ev {1, 0.0};
      THEN("we should have a 1 dimension vector with 0 magnitude") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 0.0);
      }
    }
    WHEN("we pass {1, 2.0} i.e. used and non-zero magnitude") {
      EuclideanVector ev {1, 2.0};
      THEN("we should have a 1 dimension vector with 2.0 magnitude") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 2.0);
      }
    }
    WHEN("we pass {2, 3.0}") {
      EuclideanVector ev {2, 3.0};
      THEN("we should have a 2 dimension vector with 3.0 magnitudes for all dimensions") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 3.0);
      }
    }
    WHEN("we pass {2, 3.0} and want to construct a const EV") {
      const EuclideanVector ev {2, 3.0};
      THEN("we should have a 2 dimension vector with 3.0 magnitudes for all dimensions") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 3.0);
      }
    }
    WHEN("we pass {100, 200.0} i.e. creating a much larger dimension EV") {
      EuclideanVector ev {100, 200.0};
      THEN("we should have a 100 dimension vector with 200.0 magnitudes for all dimensions") {
        REQUIRE(ev.GetNumDimensions() == 100);
        for (int i=0; i < ev.GetNumDimensions(); ++i) REQUIRE(ev[i] == 200.0);
      }
    }
  }
}

SCENARIO("The default constructor should validly construct an EV object for all valid inputs") {
  GIVEN("the default constructor") {
    WHEN("we pass no values") {
      EuclideanVector ev {};
      THEN("1 should be used as the default value for number of dimensions, magnitude 0.0") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 0.0);
      }
    }
    WHEN("we pass 1") {
      EuclideanVector ev {1};
      THEN("1 should be used as the number of dimensions and magnitude 0.0") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 0.0);
      }
    }
    WHEN("we pass a value greater than 1 e.g. 2") {
      EuclideanVector ev {2};
      THEN("2 should be used as the number of dimensions and magnitude 0.0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 0.0);
        REQUIRE(ev[1] == 0.0); // explicitly on a new line so we know which of the two fail
      }
    }
    WHEN("we pass a value greater than 1 e.g. 2 and want to create a const EV") {
      const EuclideanVector ev {2};
      THEN("2 should be used as the number of dimensions and magnitude 0.0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 0.0);
        REQUIRE(ev[1] == 0.0); // explicitly on a new line so we know which of the two fail
      }
    }
  }
}

SCENARIO("The vector iterator constructor should validly construct EV objects when"
          "given valid iterators") {
  GIVEN("an empty vector<double>") {
    std::vector<double> vec {};
    WHEN("we construct an ev via the vector's iterators") {
      EuclideanVector ev {vec.begin(), vec.end()};
      THEN("the EV should have no dimensions and consequently superfluous 0 magnitude") {
        REQUIRE(ev.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("a single element vector<double>") {
    std::vector<double> vec {5.0};
    WHEN("we construct an ev via the vector's iterators") {
      EuclideanVector ev {vec.begin(), vec.end()};
      THEN("the EV should have 1 dimension with magnitude 5.0") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 5.0);
      }
    }
  }
  GIVEN("a multi-element (2) vector<double> with identical elements") {
    std::vector<double> vec {6.0, 6.0};
    WHEN("we construct an ev via the vector's iterators") {
      EuclideanVector ev {vec.begin(), vec.end()};
      THEN("the EV should have 2 dimensions, both with magnitude 6.0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 6.0);
        REQUIRE(ev[1] == 6.0); // again, explicitly separate from above statement
      }
    }
  }
  GIVEN("a multi-element (3) vector<double> with differing elements") {
    std::vector<double> vec {7.0, 8.0, 2.0};
    WHEN("we construct an ev via the vector's iterators") {
      EuclideanVector ev {vec.begin(), vec.end()};
      THEN("the EV should have 3 dimensions with differing magnitudes, 7.0, 8.0 and 9.0") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(ev[0] == 7.0);
        REQUIRE(ev[1] == 8.0); 
        REQUIRE(ev[2] == 2.0); // again, explicitly separate from above statement
      }
    }
  }
  GIVEN("a multi-element (3) vector<double> with differing elements") {
    std::vector<double> vec {7.0, 8.0, 2.0};
    WHEN("we construct a CONST ev via the vector's iterators") {
      const EuclideanVector ev {vec.begin(), vec.end()};
      THEN("the EV should have 3 dimensions with differing magnitudes, 7.0, 8.0 and 9.0") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(ev[0] == 7.0);
        REQUIRE(ev[1] == 8.0); 
        REQUIRE(ev[2] == 2.0); // again, explicitly separate from above statement
      }
    }
  }
  GIVEN("a multi-element (3) CONST vector<double> with differing elements") {
    const std::vector<double> vec {7.0, 8.0, 2.0};
    WHEN("we construct an ev via the vector's iterators") {
      EuclideanVector ev {vec.cbegin(), vec.cend()};
      THEN("the EV should have 3 dimensions with differing magnitudes, 7.0, 8.0 and 9.0") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(ev[0] == 7.0);
        REQUIRE(ev[1] == 8.0); 
        REQUIRE(ev[2] == 2.0); // again, explicitly separate from above statement
      }
    }
  }
}

SCENARIO("the copy constructor should validly construct EV objects that are copies of existing"
          " EV objects") {

  GIVEN("already constructed non-const EV's") {
    EuclideanVector ev01 {0}; // 0d
    EuclideanVector ev02 {1}; // 1d, 0 mag
    EuclideanVector ev03 {2}; // 2d, 0 mag
    EuclideanVector ev04 {1, 2.0}; // 1d non-zero mag
    EuclideanVector ev05 {2, 2.0}; // 2d non-zero same mag
    std::vector<double> vec {3.0, 4.0}; 
    EuclideanVector ev06 {vec.begin(), vec.end()}; // 2d non-zero diff mag 
    WHEN("we copy construct") {
      EuclideanVector ev11 {ev01};
      EuclideanVector ev12 {ev02};
      EuclideanVector ev13 {ev03};
      EuclideanVector ev14 {ev04};
      EuclideanVector ev15 {ev05};
      EuclideanVector ev16 {ev06};
      THEN("we should get EV's with copied fields") {
        REQUIRE(ev11.GetNumDimensions() == 0);

        REQUIRE(ev12.GetNumDimensions() == 1);
        REQUIRE(ev12[0] == 0.0);

        REQUIRE(ev13.GetNumDimensions() == 2);
        REQUIRE(ev13[0] == 0.0);

        REQUIRE(ev14.GetNumDimensions() == 1);
        REQUIRE(ev14[0] == 2.0);

        REQUIRE(ev15.GetNumDimensions() == 2);
        REQUIRE(ev15[0] == 2.0);
        REQUIRE(ev15[1] == 2.0);

        REQUIRE(ev16.GetNumDimensions() == 2);
        REQUIRE(ev16[0] == 3.0);
        REQUIRE(ev16[1] == 4.0);
      }
    }
  }
  GIVEN("already constructed const EV's") {
    const EuclideanVector ev01 {0}; // 0d
    const EuclideanVector ev02 {1}; // 1d, 0 mag
    const EuclideanVector ev03 {2}; // 2d, 0 mag
    const EuclideanVector ev04 {1, 2.0}; // 1d non-zero mag
    const EuclideanVector ev05 {2, 2.0}; // 2d non-zero same mag
    const std::vector<double> vec {3.0, 4.0}; 
    const EuclideanVector ev06 {vec.cbegin(), vec.cend()}; // 2d non-zero diff mag 
    WHEN("we copy construct to non-const EV's") {
      EuclideanVector ev11 {ev01};
      EuclideanVector ev12 {ev02};
      EuclideanVector ev13 {ev03};
      EuclideanVector ev14 {ev04};
      EuclideanVector ev15 {ev05};
      EuclideanVector ev16 {ev06};
      THEN("this should happed") {
        REQUIRE(ev11.GetNumDimensions() == 0);

        REQUIRE(ev12.GetNumDimensions() == 1);
        REQUIRE(ev12[0] == 0.0);

        REQUIRE(ev13.GetNumDimensions() == 2);
        REQUIRE(ev13[0] == 0.0);

        REQUIRE(ev14.GetNumDimensions() == 1);
        REQUIRE(ev14[0] == 2.0);

        REQUIRE(ev15.GetNumDimensions() == 2);
        REQUIRE(ev15[0] == 2.0);
        REQUIRE(ev15[1] == 2.0);

        REQUIRE(ev16.GetNumDimensions() == 2);
        REQUIRE(ev16[0] == 3.0);
        REQUIRE(ev16[1] == 4.0);
      }
    }
  }
  GIVEN("an already constructed const EV") {
    const EuclideanVector ev1 {2, 2.0}; // 2d non-zero same mag
    WHEN("copy construct another const EV") {
      const EuclideanVector ev2 {ev1};
      THEN("ev2 should be a copy of ev1") {
        REQUIRE(ev2.GetNumDimensions() == 2);
        REQUIRE(ev2[0] == 2.0);
        REQUIRE(ev2[1] == 2.0);
      }
    }
  }
}

SCENARIO("the move constructor should re-use the resoures of pre-existing EV's"
          " to create new valid EV's whilst leaving the originals valid") {

  GIVEN("a pre-constructed l-value EV") {
    EuclideanVector recyclee {2, 3.0};
    WHEN("we create a new ev by moving from the old one") {
      EuclideanVector ev {std::move(recyclee)};
      THEN("the new ev should have the resources of the old") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 3.0);
        REQUIRE(ev[1] == 3.0);
      }
      AND_THEN("the moved from ev should have 0 dimensions") {
        REQUIRE(recyclee.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("an in-line constructed r-value EV") {
    WHEN("we create a new ev via this r-value EV") {
      EuclideanVector ev { EuclideanVector{2, 4.0} };
      THEN("the new ev should have the resources of the old") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 4.0);
        REQUIRE(ev[1] == 4.0);
      }
    }
  }
  GIVEN("a const ev") {
    const EuclideanVector cev = {2, 7.0};
    WHEN("we create a new ev via moving this const ev") {
      EuclideanVector ev { std::move(cev) };
      THEN("the new ev should be created by copy instead") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 7.0);
        REQUIRE(ev[1] == 7.0);
        REQUIRE(cev.GetNumDimensions() == 2);
      }
    }
  }
}

/* dtor testing - this probably doesn't need to be done but oh well */
SCENARIO("the default destructor should validly destruct all types of EV's") {

  GIVEN("various types of EV's") {
    WHEN("the EV's have gone out of scope and been destructed") {
      {
        EuclideanVector ev01 {0};
        EuclideanVector ev02 {1};
        EuclideanVector ev03 {2};
        EuclideanVector ev04 {0, 0.0};
        EuclideanVector ev05 {0, 1.0};
        EuclideanVector ev06 {0, 2.0};
        EuclideanVector ev07 {1, 0.0};
        EuclideanVector ev08 {1, 1.0};
        EuclideanVector ev09 {1, 2.0};
        EuclideanVector ev10 {2, 0.0};
        EuclideanVector ev11 {2, 1.0};
        EuclideanVector ev12 {2, 2.0};
      }
      THEN("no exceptions should be thrown") {
      }
    }
  }
}

/* testing operator overloads */

SCENARIO("the copy assignment operator =") {

  GIVEN("two existing EV's of the same dimensions and same uniform magnitudes") {
    EuclideanVector ev1 = {2, 5.0};
    EuclideanVector ev2 = {2, 5.0};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be a new copy of ev2 and ev2 should remain unchanged") {
        REQUIRE(ev1.GetNumDimensions() == 2);
        REQUIRE(ev1[0] == 5.0);
        REQUIRE(ev1[1] == 5.0);
      }
    }
  }
  GIVEN("two existing EV's of the same dimensions and differing uniform magnitudes") {
    EuclideanVector ev1 = {2, 5.0};
    EuclideanVector ev2 = {2, 7.0};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be a new copy of ev2 and ev2 should remain unchanged") {
        REQUIRE(ev1.GetNumDimensions() == 2);
        REQUIRE(ev1[0] == 7.0);
        REQUIRE(ev1[1] == 7.0);
      }
    }
  }
  /* these ones have the nice added benefit of testing that new memory is
   * actually allocated as a side-effect (smaller to larger dimension) */
  GIVEN("two existing EV's of different dimensions (copied from ev is larger)"
          "and differing uniform magnitudes") {
    EuclideanVector ev1 = {1, 4.0};
    EuclideanVector ev2 = {3, 9.0};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be increased in size to ev2's size, and have the same data") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 9.0);
        REQUIRE(ev1[1] == 9.0);
        REQUIRE(ev1[2] == 9.0);
      }
    }
  }
  GIVEN("two existing EV's of different dimensions (copied from ev is smaller)"
          "and differing uniform magnitudes") {
    EuclideanVector ev1 = {3, 4.0};
    EuclideanVector ev2 = {1, 2.0};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be decreased in size to ev2's size, and have the same data") {
        REQUIRE(ev1.GetNumDimensions() == 1);
        REQUIRE(ev1[0] == 2.0);
      }
    }
  }
  GIVEN("two existing EV's of same size with differing non-uniform magnitudes") {
    std::vector<double> vec1 {1.0, 2.0, 3.0};
    EuclideanVector ev1 = {vec1.begin(), vec1.end()};
    std::vector<double> vec2 {9.0, 8.0, 7.0};
    EuclideanVector ev2 = {vec2.begin(), vec2.end()};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be a new copy of ev2 and ev2 should remain unchanged") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 9.0);
        REQUIRE(ev1[1] == 8.0);
        REQUIRE(ev1[2] == 7.0);
      }
    }
  }
  GIVEN("two existing EV's of different size with differing magnitudes") {
    EuclideanVector ev1 = {1, 2.0};
    std::vector<double> vec2 {5.0, 4.0, 3.0};
    EuclideanVector ev2 = {vec2.begin(), vec2.end()};
    WHEN("we copy assign ev1 to ev2") {
      ev1 = ev2;
      THEN("ev1 should be a new copy of ev2 and ev2 should remain unchanged") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 5.0);
        REQUIRE(ev1[1] == 4.0);
        REQUIRE(ev1[2] == 3.0);
      }
    }
  }
}

SCENARIO("the move assignment operator should move the contents of an existing"
          " argument EV into an existing caller EV after deleting its current data") {

  GIVEN("two existing EV's of the same dimensions and same uniform magnitudes") {
    EuclideanVector ev1 = {2, 5.0};
    EuclideanVector ev2 = {2, 5.0};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should repurpose ev2's data and ev2 should have 0 dimensions") {
        REQUIRE(ev1.GetNumDimensions() == 2);
        REQUIRE(ev1[0] == 5.0);
        REQUIRE(ev1[1] == 5.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("two existing EV's of the same dimensions and differing uniform magnitudes") {
    EuclideanVector ev1 = {2, 5.0};
    EuclideanVector ev2 = {2, 7.0};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should repurpose ev2's data and ev2 should have 0 dimensions") {
        REQUIRE(ev1.GetNumDimensions() == 2);
        REQUIRE(ev1[0] == 7.0);
        REQUIRE(ev1[1] == 7.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("one existing EV") {
    EuclideanVector ev1 = {2, 5.0};
    WHEN("we move assign ev1 to an r-value ev of differing dimension and magnitudes") {
      std::vector<double> vec {4.0, 2.0, 0.0};
      ev1 = EuclideanVector{vec.begin(), vec.end()};
      THEN("ev1 should repurpose ev2's data and ev2 doesn't exist anymore") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 4.0);
        REQUIRE(ev1[1] == 2.0);
        REQUIRE(ev1[2] == 0.0);
      }
    }
  }
  /* these ones have the nice added benefit of testing that the memory is
   * actually "moved" as a side-effect (smaller to larger dimension) */
  GIVEN("two existing EV's of different dimensions (moved from ev is larger)"
          "and differing uniform magnitudes") {
    EuclideanVector ev1 = {1, 4.0};
    EuclideanVector ev2 = {3, 9.0};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should become larger, repurposing ev2's data. ev2 should have 0 dimensions") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 9.0);
        REQUIRE(ev1[1] == 9.0);
        REQUIRE(ev1[2] == 9.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("two existing EV's of different dimensions (copied from ev is smaller)"
          "and differing uniform magnitudes") {
    EuclideanVector ev1 = {3, 4.0};
    EuclideanVector ev2 = {1, 2.0};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should become smaller, repurposing ev2's data. ev2 should be 0 dimensional") {
        REQUIRE(ev1.GetNumDimensions() == 1);
        REQUIRE(ev1[0] == 2.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("two existing EV's of same size with differing non-uniform magnitudes") {
    std::vector<double> vec1 {1.0, 2.0, 3.0};
    EuclideanVector ev1 = {vec1.begin(), vec1.end()};
    std::vector<double> vec2 {9.0, 8.0, 7.0};
    EuclideanVector ev2 = {vec2.begin(), vec2.end()};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should repurpose ev2's data and ev2 should be 0 dimensional") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 9.0);
        REQUIRE(ev1[1] == 8.0);
        REQUIRE(ev1[2] == 7.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("two existing EV's of different size with differing magnitudes") {
    EuclideanVector ev1 = {1, 2.0};
    std::vector<double> vec2 {5.0, 4.0, 3.0};
    EuclideanVector ev2 = {vec2.begin(), vec2.end()};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should repurpose ev2's data and ev2 should be 0 dimensional") {
        REQUIRE(ev1.GetNumDimensions() == 3);
        REQUIRE(ev1[0] == 5.0);
        REQUIRE(ev1[1] == 4.0);
        REQUIRE(ev1[2] == 3.0);
        REQUIRE(ev2.GetNumDimensions() == 0);
      }
    }
  }
  GIVEN("a const and non-const ev") {
    EuclideanVector ev1 = {2, 5.0};
    const EuclideanVector ev2 = {2, 7.0};
    WHEN("we move assign ev1 to ev2") {
      ev1 = std::move(ev2);
      THEN("ev1 should be copy assigned from ev2 because it is const") {
        REQUIRE(ev1.GetNumDimensions() == 2);
        REQUIRE(ev1[0] == 7.0);
        REQUIRE(ev1[1] == 7.0);
        REQUIRE(ev2.GetNumDimensions() == 2);
      }
    }
  }
}

SCENARIO("the subscript operator [] should have getter/setter semantics for non-const ev's "
          "but only getter semantics for const ev's") {

  GIVEN("a 1d non-const ev with unit magnitude") {
    EuclideanVector ev {1, 1.0};
    WHEN("we subscript access this ev and save the return in a reference") {
      double& dr = ev[0];
      THEN("the reference should be of unit value") {
        REQUIRE(dr == 1.0);
      }
      AND_THEN("we should be able to set the value at the accessed index via this reference") {
        dr = 6.0;
        REQUIRE(ev[0] == 6.0);
      }
    }
    AND_WHEN("we directly set the subcript access equal to a valid magnitude value") {
      ev[0] = 9.0;
      THEN("the value at the accessed index should have been updated") {
        REQUIRE(ev[0] == 9.0);
      }
    }
  }
  GIVEN("an n dimensional non-const ev with non-uniform magnitudes") {
    std::vector<double> vec = {5.0, 7.0, 3.0};
    EuclideanVector ev {vec.begin(), vec.end()};
    WHEN("we access the 0-indexed n indices we should be returned the value at those indices") {
      REQUIRE(ev[0] == 5.0);
      REQUIRE(ev[1] == 7.0);
      REQUIRE(ev[2] == 3.0);
      AND_WHEN("we use the subscript as the LHS of an assignment") {
        ev[0] = 11.0;
        ev[1] = 22.0;
        ev[2] = 14.0;
        THEN("the RHS values should be assigned to these respective indices") {
          REQUIRE(ev[0] == 11.0);
          REQUIRE(ev[1] == 22.0);
          REQUIRE(ev[2] == 14.0);
        }
      }
    }
    AND_WHEN("we use subscripts as both the LHS and RHS of an assignment") {
      ev[2] = 6.0;
      REQUIRE(ev[2] == 6.0);
      REQUIRE(ev[0] != 6.0);
      ev[0] = ev[2];
      THEN("the LHS subscript indice should have the same value as the RHS subscript indice") {
        REQUIRE(ev[0] == 6.0);
      }
    }
  }
  GIVEN("a const ev") {
    const EuclideanVector ev {2, 3.0};
    WHEN("we subscipt access this const ev and store the return in a non-const variable") {
      double mag_cpy = ev[0];
      THEN("the double should have the value at the indexed indice") {
        REQUIRE(mag_cpy == 3.0);
        AND_WHEN("we change the value of this variable") {
          mag_cpy = 7.0;
          THEN("the ev at the originally indexed indice should not change") {
            REQUIRE(mag_cpy == 7.0);
            REQUIRE(ev[0] == 3.0);
          }
        }
      }
    }
    AND_WHEN("we store the subscript access return in a const variable") {
      const double c_mag_cpy = ev[1];
      THEN("the const variable should have the value at that index") {
        REQUIRE(c_mag_cpy == 3.0);
      }
    }
  }
}

/* NOTE: after seeing the spec  */

SCENARIO("operator +=") {

  GIVEN("two EV's of different dimension") {
    EuclideanVector a = {2, 1.0};
    EuclideanVector b {3, 5.0};
    WHEN("we try to += them") {
       REQUIRE_THROWS_WITH(a += b, "Dimensions of LHS(2) and RHS(3) do not match");
    }
  }
  GIVEN("two EV's of the same dimension and uniform magnitudes") {
    EuclideanVector a = {2, 1.0};
    EuclideanVector b {2, 5.0};
    WHEN("we try to += them") {
        a += b;
        THEN("a should have b's magnitudes added to it, and b unchanged") {
          REQUIRE(a.GetNumDimensions() == 2);
          REQUIRE(a[0] == 6.0);
          REQUIRE(a[1] == 6.0);
          REQUIRE(b.GetNumDimensions() == 2);
          REQUIRE(b[0] == 5.0);
          REQUIRE(b[1] == 5.0);
       }
    }
  }
  GIVEN("two EV's of the same dimension and different magnitudes") {
    EuclideanVector a {2};
    EuclideanVector b {2};
    a[0] = 2.0;
    a[1] = 4.0;
    b[0] = 5.0;
    b[1] = 10.0;
    WHEN("we try to += them") {
        a += b;
        THEN("a should have b's magnitudes added to it, and b unchanged") {
          REQUIRE(a.GetNumDimensions() == 2);
          REQUIRE(a[0] == 7.0);
          REQUIRE(a[1] == 14.0);
          REQUIRE(b.GetNumDimensions() == 2);
          REQUIRE(b[0] == 5.0);
          REQUIRE(b[1] == 10.0);
       }
    }
  }
  GIVEN("a const RHS ev") {
    EuclideanVector a = {2, 1.0};
    a[1] = 3.0;
    const EuclideanVector b {2, 2.0};
    WHEN("we try to += them") {
      a += b;
      THEN("a should have b's magnitudes added to it") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 3.0);
        REQUIRE(a[1] == 5.0);
        // not crashing is enough to know b wasn't changed
      }
    }
  }
}

SCENARIO("operator -=") {

  GIVEN("two EV's of different dimension") {
    EuclideanVector a = {2, 1.0};
    EuclideanVector b {3, 5.0};
    WHEN("we try to -= them") {
       REQUIRE_THROWS_WITH(a -= b, "Dimensions of LHS(2) and RHS(3) do not match");
    }
  }
  GIVEN("two EV's of the same dimension and uniform magnitudes") {
    EuclideanVector a = {2, 5.0};
    EuclideanVector b {2, 2.0};
    WHEN("we try to -= them") {
        a -= b;
        THEN("a should have b's magnitudes subtracted from it, and b unchanged") {
          REQUIRE(a.GetNumDimensions() == 2);
          REQUIRE(a[0] == 3.0);
          REQUIRE(a[1] == 3.0);
          REQUIRE(b.GetNumDimensions() == 2);
          REQUIRE(b[0] == 2.0);
          REQUIRE(b[1] == 2.0);
       }
    }
  }
  GIVEN("two EV's of the same dimension and different magnitudes") {
    EuclideanVector a {2};
    EuclideanVector b {2};
    a[0] = 5.0;
    a[1] = 10.0;
    b[0] = 2.0;
    b[1] = 4.0;
    WHEN("we try to -= them") {
        a -= b;
        THEN("a should have b's magnitudes subtracted from it, and b unchanged") {
          REQUIRE(a.GetNumDimensions() == 2);
          REQUIRE(a[0] == 3.0);
          REQUIRE(a[1] == 6.0);
          REQUIRE(b.GetNumDimensions() == 2);
          REQUIRE(b[0] == 2.0);
          REQUIRE(b[1] == 4.0);
       }
    }
  }
  GIVEN("a const RHS ev") {
    EuclideanVector a = {2, 6.0};
    a[1] = 3.0;
    const EuclideanVector b {2, 2.0};
    WHEN("we try to -= them") {
      a -= b;
      THEN("a should have b's magnitudes subtracted from it") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 4.0);
        REQUIRE(a[1] == 1.0);
        // not crashing is enough to know b wasn't changed
      }
    }
  }
}

SCENARIO("operator *=") {

  GIVEN("a UV with magnitudes of 0.0") {
    EuclideanVector ev {2};
    WHEN("we multiply by any scaler") {
      ev *= 2;
      THEN("we should still have magnitudes of 0.0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 0.0);
        REQUIRE(ev[1] == 0.0);
      }
    }
  }
  GIVEN("a UV with non-zero magnitudes") {
    EuclideanVector ev {2, 3.0};
    ev[1] = 5.0;
    WHEN("we multiply by any positive scaler") {
      ev *= 2;
      THEN("each magnitude should be scaled by that scaler") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 6.0);
        REQUIRE(ev[1] == 10.0);
      }
    }
    AND_WHEN("we multiply by any negative scaler") {
      ev *= -2;
      THEN("each magnitude should be scaled by that scaler") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == -6.0);
        REQUIRE(ev[1] == -10.0);
      }
    }
    AND_WHEN("we multiply by a zero scaler") {
      ev *= 0;
      THEN("each magnitude should be 0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 0);
        REQUIRE(ev[1] == 0);
      }
    }
  }
}

SCENARIO("operator /=") {

  GIVEN("a vector") {
    EuclideanVector ev = {2, 1.0};
    WHEN("we try to divide with a 0 scaler") {
       REQUIRE_THROWS_WITH(ev /= 0, "Invalid vector division by 0");
    }
  }

  GIVEN("a UV with magnitudes of 0.0") {
    EuclideanVector ev {2};
    WHEN("we divide by any valid scaler") {
      ev /= 2;
      THEN("we should still have magnitudes of 0.0") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 0.0);
        REQUIRE(ev[1] == 0.0);
      }
    }
  }
  GIVEN("a UV with non-zero magnitudes") {
    EuclideanVector ev {2, 4.0};
    ev[1] = 6.0;
    WHEN("we divide by any positive scaler") {
      ev /= 2;
      THEN("each magnitude should be divided by that scaler") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == 2.0);
        REQUIRE(ev[1] == 3.0);
      }
    }
    AND_WHEN("we divide by any negative scaler") {
      ev /= -2;
      THEN("each magnitude should be divided by that scaler") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(ev[0] == -2.0);
        REQUIRE(ev[1] == -3.0);
      }
    }
  }
}

SCENARIO("operator std::vector<double>") {

  GIVEN("a zero dimensional ev") {
    EuclideanVector ev {0};
    WHEN("we cast this ev to a vector type") {
      std::vector<double> vec = static_cast<std::vector<double>>(ev);
      THEN("this vec should contain no elements") {
        REQUIRE(vec.size() == 0);
      }
    }
  }
  GIVEN("a 1d ev") {
    EuclideanVector ev {1, 2.0};
    WHEN("we cast this ev to a vector type") {
      std::vector<double> vec = static_cast<std::vector<double>>(ev);
      THEN("this vec should contain one element of magnitude 2") {
        REQUIRE(vec.size() == 1);
        REQUIRE(vec.at(0) == 2.0);
      }
    }
  }
  GIVEN("a multi-dimensional ev") {
    EuclideanVector ev {3, 1.0};
    ev[1] = 3.0;
    ev[2] = 2.0;
    WHEN("we cast this ev to a vector type") {
      std::vector<double> vec = static_cast<std::vector<double>>(ev);
      THEN("this vec should contain three elements of corresponding magnitude") {
        REQUIRE(vec.size() == 3);
        REQUIRE(vec.at(0) == 1.0);
        REQUIRE(vec.at(1) == 3.0);
        REQUIRE(vec.at(2) == 2.0);
      }
    }
  }
  GIVEN("a 1d ev") {
    EuclideanVector ev {1, 2.0};
    WHEN("we cast this ev to a const vector type") {
      const std::vector<double> vec = static_cast<std::vector<double>>(ev);
      THEN("this vec should contain one element of magnitude 2") {
        REQUIRE(vec.size() == 1);
        REQUIRE(vec.at(0) == 2.0);
      }
    }
  }
}

SCENARIO("operator std::list<double>") {

  GIVEN("a zero dimensional ev") {
    EuclideanVector ev {0};
    WHEN("we cast this ev to a list type") {
      std::list<double> list = static_cast<std::list<double>>(ev);
      THEN("this list should contain no elements") {
        REQUIRE(list.size() == 0);
      }
    }
  }
  GIVEN("a 1d ev") {
    EuclideanVector ev {1, 2.0};
    WHEN("we cast this ev to a list type") {
      std::list<double> list = static_cast<std::list<double>>(ev);
      THEN("this list should contain one element of magnitude 2") {
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 2.0);
      }
    }
  }
  GIVEN("a multi-dimensional ev") {
    EuclideanVector ev {3, 1.0};
    ev[1] = 3.0;
    ev[2] = 2.0;
    WHEN("we cast this ev to a list type") {
      std::list<double> list = static_cast<std::list<double>>(ev);
      THEN("this list should contain three elements of corresponding magnitude") {
        REQUIRE(list.size() == 3);
        std::_List_iterator it = list.begin();
        REQUIRE(*it++ == 1.0);
        REQUIRE(*it++  == 3.0);
        REQUIRE(*it == 2.0);
      }
    }
  }
  GIVEN("a 1d ev") {
    EuclideanVector ev {1, 2.0};
    WHEN("we cast this ev to a const list type") {
      const std::list<double> list = static_cast<std::list<double>>(ev);
      THEN("this list should contain one element of magnitude 2") {
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 2.0);
      }
    }
  }
}

/* allowing testing both of these at once as != calls == */
SCENARIO("friend operator == and !=") {

  GIVEN("one vector") {
    EuclideanVector ev {1};
    WHEN("we compare equal to itself") {
      REQUIRE(ev == ev);
      REQUIRE_FALSE(ev != ev);
    }
  }
  GIVEN("two vectors of same dimension but different magnitudes") {
    EuclideanVector a {1, 1.0};
    EuclideanVector b {1, 2.0};
    WHEN("we compare them equal") {
      REQUIRE_FALSE(a == b);
      REQUIRE(a != b);
    }
  }
  GIVEN("two vectors of same dimension and same magnitudes") {
    EuclideanVector a {2, 3.0};
    EuclideanVector b {2, 3.0};
    a[1] = 4.0;
    b[1] = 4.0;
    WHEN("we compare them equal") {
      REQUIRE(a == b);
      REQUIRE_FALSE(a != b);
    }
  }
  GIVEN("two vectors of 0 dimension") {
    EuclideanVector a {0};
    EuclideanVector b {0};
    WHEN("we compare them equal") {
      REQUIRE(a == b);
      REQUIRE_FALSE(a != b);
    }
  }
}

SCENARIO("friend operator +") {

  GIVEN("two ev's of different dimensions") {
    EuclideanVector a {1, 1.0};
    EuclideanVector b {2, 2.0};
    WHEN("we try to add these") {
      REQUIRE_THROWS_WITH(a + b, "Dimensions of LHS(1) and RHS(2) do not match");
    }
  }
  GIVEN("two ev's of the same dimensions") {
    EuclideanVector a {2, 1.0};
    EuclideanVector b {2, 2.0};
    b[1] = 3.0;
    WHEN("we add these together and store in another ev") {
      EuclideanVector c = a + b;
      THEN("a and b should be unmodified, c should contain a + b mags") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 1.0);
        REQUIRE(a[1] == 1.0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == 2.0);
        REQUIRE(b[1] == 3.0);
        REQUIRE(c.GetNumDimensions() == 2);
        REQUIRE(c[0] == 3.0);
        REQUIRE(c[1] == 4.0);
      }
    }
  }
  GIVEN("two vectors of 0 dimension") {
    EuclideanVector a {0};
    EuclideanVector b {0};
    WHEN("we add them") {
      EuclideanVector c = a + b;
      REQUIRE(c.GetNumDimensions() == 0);
    }
  }
}

SCENARIO("friend operator -") {

  GIVEN("two ev's of different dimensions") {
    EuclideanVector a {1, 1.0};
    EuclideanVector b {2, 2.0};
    WHEN("we try to subtract these") {
      REQUIRE_THROWS_WITH(a - b, "Dimensions of LHS(1) and RHS(2) do not match");
    }
  }
  GIVEN("two ev's of the same dimensions") {
    EuclideanVector a {2, 6.0};
    EuclideanVector b {2, 2.0};
    b[1] = 3.0;
    WHEN("we subtract b from a and store result in another ev") {
      EuclideanVector c = a - b;
      THEN("a and b should be unmodified, c should contain a - b mags") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 6.0);
        REQUIRE(a[1] == 6.0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == 2.0);
        REQUIRE(b[1] == 3.0);
        REQUIRE(c.GetNumDimensions() == 2);
        REQUIRE(c[0] == 4.0);
        REQUIRE(c[1] == 3.0);
      }
    }
  }
  GIVEN("two vectors of 0 dimension") {
    EuclideanVector a {0};
    EuclideanVector b {0};
    WHEN("we subtract them") {
      EuclideanVector c = a - b;
      REQUIRE(c.GetNumDimensions() == 0);
    }
  }
}

SCENARIO("friend operator ev * ev") {

  GIVEN("two ev's of different dimensions") {
    EuclideanVector a {1, 1.0};
    EuclideanVector b {2, 2.0};
    WHEN("we try to multiply these") {
      REQUIRE_THROWS_WITH(a * b, "Dimensions of LHS(1) and RHS(2) do not match");
    }
  }
  GIVEN("two ev's of the same dimensions") {
    EuclideanVector a {2, 1.0};
    EuclideanVector b {2, 3.0};
    a[1] = 2.0;
    b[1] = 4.0;
    WHEN("we multiply (dot product) a and b and store in a variable") {
      double dot_prod = a * b;
      THEN("a and b should be unmodified, dot product should be sum of pair products") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 1.0);
        REQUIRE(a[1] == 2.0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == 3.0);
        REQUIRE(b[1] == 4.0);
        REQUIRE(dot_prod == 11);
      }
    }
  }
  GIVEN("two vectors of 0 dimension") {
    EuclideanVector a {0};
    EuclideanVector b {0};
    WHEN("we dot product them") {
      double dot_prod = a * b;
      REQUIRE(dot_prod == 0);
    }
  }
}

/* again, testing these in tandem because one calls the other */
SCENARIO("friend operator scaler * ev && ev * scaler") {

  GIVEN("a vector with 0 mags") {
    EuclideanVector ev {2, 0.0};
    WHEN("we multiply this ev by a scaler") {
      EuclideanVector a = ev * 2;
      EuclideanVector b = 2 * ev;
      THEN("we should still have 0 mags") {
        REQUIRE(ev[0] == 0.0);
        REQUIRE(ev[1] == 0.0);
        REQUIRE(a[0] == 0.0);
        REQUIRE(a[1] == 0.0);
        REQUIRE(b[0] == 0.0);
        REQUIRE(b[1] == 0.0);
      }
    }
  }
  GIVEN("a EV with non-zero magnitudes") {
    EuclideanVector ev {2, 3.0};
    ev[1] = 5.0;
    WHEN("we multiply by any positive scaler") {
      EuclideanVector a = ev * 2;
      EuclideanVector b = 2 * ev;
      THEN("each magnitude should be scaled by that scaler") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 6.0);
        REQUIRE(a[1] == 10.0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == 6.0);
        REQUIRE(b[1] == 10.0);
      }
    }
    AND_WHEN("we multiply by any negative scaler") {
      EuclideanVector a = ev * -2;
      EuclideanVector b = -2 * ev;
      THEN("each magnitude should be scaled by that scaler") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == -6.0);
        REQUIRE(a[1] == -10.0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == -6.0);
        REQUIRE(b[1] == -10.0);
      }
    }
    AND_WHEN("we multiply by a zero scaler") {
      EuclideanVector a = ev * 0;
      EuclideanVector b = 0 * ev;
      THEN("each magnitude should be scaled by that scaler") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 0);
        REQUIRE(a[1] == 0);
        REQUIRE(b.GetNumDimensions() == 2);
        REQUIRE(b[0] == 0);
        REQUIRE(b[1] == 0);
      }
    }
  }
}

SCENARIO("friend operator ev / scaler") {

  GIVEN("any EV") {
    EuclideanVector a {2, 2.0};
    WHEN("we try to divide by 0") {
      REQUIRE_THROWS_WITH(a / 0, "Invalid vector division by 0");
    }
  }
  GIVEN("a vector with 0 mags") {
    EuclideanVector ev {2, 0.0};
    WHEN("we divide this ev by a scaler") {
      EuclideanVector a = ev / 2;
      THEN("we should still have 0 mags") {
        REQUIRE(a[0] == 0.0);
        REQUIRE(a[1] == 0.0);
      }
    }
  }
  GIVEN("a EV with non-zero magnitudes") {
    EuclideanVector ev {2, 4.0};
    ev[1] = 6.0;
    WHEN("we divide by any positive scaler") {
      EuclideanVector a = ev / 2;
      THEN("each magnitude should be divided by that scaler") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == 2.0);
        REQUIRE(a[1] == 3.0);
      }
    }
    AND_WHEN("we multiply by any negative scaler") {
      EuclideanVector a = ev / -2;
      THEN("each magnitude should be divided by that scaler") {
        REQUIRE(a.GetNumDimensions() == 2);
        REQUIRE(a[0] == -2.0);
        REQUIRE(a[1] == -3.0);
      }
    }
  }
}

SCENARIO("friend operator <<") {

  GIVEN("a 0 dimension vector") {
    EuclideanVector ev {0};
    std::stringstream ss;
    WHEN("we print it") {
      ss << ev;
      REQUIRE(ss.str() == "[]");
    }
  }
  GIVEN("a 1 dimension vector") {
    EuclideanVector ev {1, 1.0};
    std::stringstream ss;
    WHEN("we print it") {
      ss << ev;
      REQUIRE(ss.str() == "[1]");
    }
  }
  GIVEN("a 2 dimension vector") {
    EuclideanVector ev {2, 2.0};
    ev[0] = 1;
    std::stringstream ss;
    WHEN("we print it") {
      ss << ev;
      REQUIRE(ss.str() == "[1 2]");
    }
  }
  GIVEN("a 3 dimension vector") {
    EuclideanVector ev {3, 3.0};
    std::stringstream ss;
    ev[1] = 1;
    ev[2] = 2;
    WHEN("we print it") {
      ss << ev;
      REQUIRE(ss.str() == "[3 1 2]");
    }
  }
  GIVEN("a vec with neg magnitude") {
    EuclideanVector ev {3, -3.0};
    std::stringstream ss;
    ev[1] = -1;
    ev[2] = 2;
    WHEN("we print it") {
      ss << ev;
      REQUIRE(ss.str() == "[-3 -1 2]");
    }
  }
}

SCENARIO("method at()") {

  GIVEN("both const and non-const 0-d vectors and a non-0d vectors") {
    EuclideanVector a {0};
    EuclideanVector b {2, 2.0};
    const EuclideanVector ca {0};
    const EuclideanVector cb {2, 2.0};
    WHEN("we try to access either out of bounds") {
      REQUIRE_THROWS_WITH(a.at(0), "Index 0 is not valid for this EuclideanVector object");
      REQUIRE_THROWS_WITH(b.at(2), "Index 2 is not valid for this EuclideanVector object");
      REQUIRE_THROWS_WITH(ca.at(0), "Index 0 is not valid for this EuclideanVector object");
      REQUIRE_THROWS_WITH(cb.at(2), "Index 2 is not valid for this EuclideanVector object");
    }
  }
  GIVEN("a non-const ev") {
    EuclideanVector a {2, 2.0};
    a[1] = 4.0;
    WHEN("we get from it") {
      double& dr0 = a.at(0);
      double& dr1 = a.at(1);
      THEN("we should be return references to the actual mags")
        REQUIRE(&dr0 == &(a.at(0)));
        REQUIRE(&dr1 == &(a.at(1)));
    }
    AND_WHEN("we set via these references, both directly and via a variable") {
      double& dr0 = a.at(0);
      dr0 = 6.0;
      a.at(1) = 9.0;
      THEN("the vector itself should be changed") {
        REQUIRE(a[0] == 6.0);
        REQUIRE(a[1] == 9.0);
      }
    }
  }
  GIVEN("a const ev") {
    const EuclideanVector a {2, 2.0};
    WHEN("we get from it") {
      double d = a.at(0);
      THEN("we should get the correct value") {
        REQUIRE(d == 2.0);
      }
    }
    AND_WHEN("we change a value we have gotten (try set)") {
      double d = a.at(0);
      d = 6.0;
      THEN("the value should change in the variable but not in the ev") {
        REQUIRE(d == 6.0);
        REQUIRE(a[0] == 2.0);
      }
    }
  }
}

/* taking the liberty to not test GetNumDimensions as other tests implicitly use and
hence test it (and it is a trivial function - a one line getter) */

SCENARIO("method GetEuclideanNorm()") {

  GIVEN("a 0-dimensional ev") {
    EuclideanVector ev {0};
    WHEN("we try get its normal") {
      REQUIRE_THROWS_WITH(ev.GetEuclideanNorm(), "EuclideanVector with no dimensions does not have a norm");
    }
  }
  GIVEN("a vec with 0-mags") {
    EuclideanVector ev {3, 0.0};
    WHEN("we get it's normal") {
      double norm = ev.GetEuclideanNorm();
      THEN("it should be 0") {
        REQUIRE(norm == 0);
      }
    }
  }
  GIVEN("a vec with 1.0 mags") {
    EuclideanVector ev {4, 1.0};
    WHEN("we get it's normal") {
      double norm = ev.GetEuclideanNorm();
      THEN("it should be the square root of the sum of mag squares") {
        REQUIRE(norm == 2);
      }
    }
  }
  GIVEN("a vec with non-zero positive mags") {
    EuclideanVector ev {4, 3.0};
    WHEN("we get it's normal") {
      double norm = ev.GetEuclideanNorm();
      THEN("it should be the square root of the sum of mag squares") {
        REQUIRE(norm == 6);
      }
    }
  }
  GIVEN("a vec with non-zero negative mags") {
    EuclideanVector ev {4, -3.0};
    WHEN("we get it's normal") {
      double norm = ev.GetEuclideanNorm();
      THEN("it should be the square root of the sum of mag squares") {
        REQUIRE(norm == 6);
      }
    }
  }
  GIVEN("a const ev") {
    const EuclideanVector ev {4, 3.0};
    WHEN("we get it's normal") {
      double norm = ev.GetEuclideanNorm();
      THEN("it should be the square root of the sum of mag squares") {
        REQUIRE(norm == 6);
      }
    }
  }
}

SCENARIO("method CreateUnitVector") {

  GIVEN("a 0-dimensional ev") {
    EuclideanVector ev {0};
    WHEN("we try create its unit vector") {
      REQUIRE_THROWS_WITH(ev.CreateUnitVector(), "EuclideanVector with no dimensions does not have a unit vector");
    }
  }
  GIVEN("a non-0-dimensional ev with a normal of 0") {
    EuclideanVector ev {3, 0.0};
    REQUIRE(ev.GetEuclideanNorm() == 0);
    WHEN("we try create its unit vector") {
      REQUIRE_THROWS_WITH(ev.CreateUnitVector(), "EuclideanVector with euclidean normal of 0 does not have a unit vector");
    }
  }
  GIVEN("a non-0-dimensional ev with a non-zero normal") {
    EuclideanVector ev {4, 2.0};
    REQUIRE(ev.GetEuclideanNorm() == 4);
    WHEN("we create its unit vector") {
      EuclideanVector unit_vec = ev.CreateUnitVector();
      THEN("the original vector should be unchanged and unit vec values correct") {
        REQUIRE(ev.GetNumDimensions() == 4);
        REQUIRE(ev[0] == 2);
        REQUIRE(ev[1] == 2);
        REQUIRE(ev[2] == 2);
        REQUIRE(ev[3] == 2); 
        REQUIRE(unit_vec.GetNumDimensions() == 4);
        REQUIRE(unit_vec[0] == 0.5);
        REQUIRE(unit_vec[1] == 0.5);
        REQUIRE(unit_vec[2] == 0.5);
        REQUIRE(unit_vec[3] == 0.5); 
      }
    }
  }
  GIVEN("a const ev with non-zero normal") {
    std::vector<double> vec {6, 4, -2, -8, 1};
    const EuclideanVector ev {vec.begin(), vec.end()};
    WHEN("we calculate the unit vec") {
      EuclideanVector unit_vec = ev.CreateUnitVector();
      THEN("it should be the same size as ev and ev unchanged") {
        REQUIRE(unit_vec.GetNumDimensions() == 5);
        // because const if this runs we know it isn't changed
        // can't explicitly compare == the unit vec values because of precision diffs
        unit_vec[0] = 1.0; // test that unit vec can be non-const
      }
    }
  }
}

// CPP ME
SCENARIO("what we are testing") {

  GIVEN("some setup") {

    WHEN("we do something on this setup") {

      THEN("this should happed") {
        REQUIRE(1);
      }
    }
  }
}