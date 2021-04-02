#include "../src/state/state.hh"
#include "../src/util/util.hh"


TEST_CASE("State::State")
{
  Log log = Log(1);
  State state = State(log);
  unsigned int num = state.num_;
  REQUIRE(4000 == state.num_);
  REQUIRE(1000 == state.width_);
  REQUIRE(1000 == state.height_);
  REQUIRE(Approx(Util::deg_to_rad(180)) == state.alpha_);
  REQUIRE(Approx(Util::deg_to_rad(17)) == state.beta_);
  REQUIRE(24.0f == state.scope_);
  REQUIRE(4.0f == state.speed_);
  REQUIRE(2.0f == state.prad_);
  REQUIRE(24.0f * 24.0f == state.scope_squared_);
  REQUIRE(num == state.px_.size());
  REQUIRE(num == state.py_.size());
  REQUIRE(num == state.pf_.size());
  REQUIRE(num == state.pc_.size());
  REQUIRE(num == state.ps_.size());
  REQUIRE(num == state.pn_.size());
  REQUIRE(num == state.pl_.size());
  REQUIRE(num == state.pr_.size());
  REQUIRE(num == state.gcol_.size());
  REQUIRE(num == state.grow_.size());
}

TEST_CASE("State::respawn")
{
  Log log = Log(1);
  State state = State(log);
  unsigned int num = state.num_;
  state.respawn();
  REQUIRE(num == state.px_.size());
  REQUIRE(num == state.py_.size());
  REQUIRE(num == state.pf_.size());
  REQUIRE(num == state.pc_.size());
  REQUIRE(num == state.ps_.size());
  REQUIRE(num == state.pn_.size());
  REQUIRE(num == state.pl_.size());
  REQUIRE(num == state.pr_.size());
  REQUIRE(num == state.gcol_.size());
  REQUIRE(num == state.grow_.size());
}

TEST_CASE("State::clear")
{
  Log log = Log(1);
  State state = State(log);
  state.clear();
  REQUIRE(0 == state.px_.size());
  REQUIRE(0 == state.py_.size());
  REQUIRE(0 == state.pf_.size());
  REQUIRE(0 == state.pc_.size());
  REQUIRE(0 == state.ps_.size());
  REQUIRE(0 == state.pn_.size());
  REQUIRE(0 == state.pl_.size());
  REQUIRE(0 == state.pr_.size());
  REQUIRE(0 == state.gcol_.size());
  REQUIRE(0 == state.grow_.size());
}

TEST_CASE("State::change")
{
  Log log = Log(2);
  REQUIRE(0 == log.messages_.size());
  State state = State(log);
  Stative stative = {-1,
                     state.num_,
                     state.width_,
                     state.height_,
                     state.alpha_,
                     state.beta_,
                     state.scope_,
                     state.speed_,
                     state.prad_,
                     state.coloring_};
  REQUIRE(false == state.change(stative));
  stative = {-1,
             state.num_,
             state.width_,
             state.height_,
             state.alpha_ + 0.1f,
             state.beta_,
             state.scope_,
             state.speed_,
             state.prad_,
             state.coloring_};
  REQUIRE(true == state.change(stative));
  REQUIRE(1 == log.messages_.size());
  REQUIRE("Changing state without respawn." == log.messages_.front().second);
  stative = {-1,
             state.num_ + 1,
             state.width_,
             state.height_,
             state.alpha_,
             state.beta_,
             state.scope_,
             state.speed_,
             state.prad_,
             state.coloring_};
  REQUIRE(true == state.change(stative));
  REQUIRE(2 == log.messages_.size());
  REQUIRE("Changing state and respawning." == log.messages_.front().second);
}

