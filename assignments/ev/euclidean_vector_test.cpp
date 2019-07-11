/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

  - a lot of these tests are probably considered superfluous or a bit over the top and cautious,
  the reason/justification I give for them is that i'm simply not comfortable with all the 
  different ways things like construction, copying and moving can be performed (both in terms 
  of how the code is written and how C++ does things under the hood)
  TLDR: sorry the tests are so long and a bit superfluous in some cases but it was for my
  peace of mind

  - justification : for testing copy assignment operator= overload, I do not explicitly test that
  the argument EV "const EuclideanVector& original" (i.e. the one being copied from) is unchanged
  as the const keyword should enforce this to be the case.  

*/

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

#include <vector>

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
        for (int i=0; i < ev.GetNumDimensions(); ++i) REQUIRE(ev[i] == 200.0); // ASK TUTOR: is this good or bad?
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

SCENARIO("the copy assignment operator should copy the contents of an existing"
          " argument EV into an existing caller EV") {

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
  /* this one has the nice added benefit of testing that new memory is
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
  GIVEN("two existing EV's with differing non-uniform magnitudes") {
    std::vector<double> vec1 {1.0, 2.0, 3.0};
    std::vector<double> vec2 {9.0, 8.0, 7.0};
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