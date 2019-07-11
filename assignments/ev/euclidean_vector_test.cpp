/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

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
}

/* dtor testing */
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


// CPP ME
SCENARIO("what we are testing") {

  GIVEN("some setup") {

    // do the setup
    // make vars etc.
    // REQUIRE(); some stuff
    // REQUIRE();

    WHEN("we do something on this setup") {

      // do the something

      THEN("this should happed") {
        REQUIRE(1);
        //REQUIRE(); that it happened
        //REQUIRE();

      }
    }
  }
}