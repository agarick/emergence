#include "view.hh"
#include "canvas.hh"
#include "headless.hh"


// Factory of Views

std::unique_ptr<View>
View::init(Log& log, Control& ctrl, UiState& uistate,
           bool headless, bool gui_on, bool three)
{
  State& state = ctrl.state_;
  if (headless) {
    std::unique_ptr<View> view(new Headless(log, ctrl, uistate));
    return view;
  }
  std::unique_ptr<View> view(new Canvas(log, ctrl, uistate, gui_on, three));
  return view;
}

