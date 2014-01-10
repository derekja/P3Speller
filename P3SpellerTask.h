////////////////////////////////////////////////////////////////////////////////
// $Id: P3SpellerTask.h 3901 2012-04-04 08:53:22Z mellinger $
// Authors: schalk@wadsworth.org, vkamat@cambridgeconsultants.com,
//   pbrunner@wadsworth.org, shzeng, juergen.mellinger@uni-tuebingen.de
// Description: The task filter for a P300 based speller providing multiple
//   menus.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef P3_SPELLER_TASK_H
#define P3_SPELLER_TASK_H

#include "StimulusTask.h"
#include "Speller.h"
#include "StatusBar.h"
#include "TextWindow.h"
#include "SockStream.h"
#include "lsl_cpp.h"

#include <stack>
#include <vector>
#include <ctime>

class P3SpellerTask : public StimulusTask, public Speller
{
 public:
  P3SpellerTask();
  ~P3SpellerTask();

 protected:
  // StimulusTask events
  virtual void    OnPreflight( const SignalProperties& Input ) const;
  virtual void    OnInitialize( const SignalProperties& Input );
  virtual void    OnStartRun();
  virtual void    OnStopRun();
  virtual void    OnPreSequence();
  virtual void    DoPreSequence( const GenericSignal&, bool& doProgress );
  virtual void    OnSequenceBegin();
  virtual void	  OnStimulusBegin( int stimulusCode );
  virtual void    OnPostRun();
  virtual void    DoPostSequence( const GenericSignal&, bool& doProgress );
  virtual Target* OnClassResult( const ClassResult& );
  virtual int     OnNextStimulusCode();
  // Speller events
  virtual void    OnEnter( const std::string& );
  // Speller commands dispatched from OnEnter
  virtual void    OnText( const std::string& );
  virtual void    OnBackspace();
  virtual void    OnDeleteWord();
  virtual void    OnUndo();
  virtual void    OnEnd();
  virtual void    OnSleep();
  virtual void    OnPause();
  virtual void    OnGoto( int );
  virtual void    OnBack();
  virtual void    OnSave();
  virtual void    OnRetrieve();

 private:
  void CheckSwitchMenu();
  void DetermineAttendedTarget();
  void InitSequence();
  void ClearSequence();
  void ClearTextHistory();
  // The LoadMenu function fills its output containers with stimuli/graph objects.
  void LoadMenu( int                inMenuIdx,
                 GUI::Rect&         ioRect,
                 GUI::GraphDisplay& ioDisplay,
                 SetOfStimuli&      ioStimuli,
                 AssociationMap&    ioAssociations,
                 Speller&           ioSpeller ) const;

  bool MultipleMenus() const;
  int  NumMenus() const;
  int  MenuRows( int inMenuIdx ) const;
  int  MenuCols( int inMenuIdx ) const;
  ParamRef MenuParam( const std::string& inParamName, int inMenuIdx ) const;

  // Log file, text window file.
  static std::string TimeStamp();
  static std::string StringTime();
  static std::string StringDate();
  static std::string SummaryFileExtension();
  std::string DirectoryFileName() const;

 private:
  // Configuration parameters.
  int  mNumberOfSequences,
       mInterpretMode_;
  bool mDisplayResults,
       mTestMode;
  std::string mTextToSpell;

  lsl::stream_info mInfo;
  lsl::stream_outlet mStreamOutlet;	// our stream outlet
  bool mFirstSequence;
  int mTargetRow, mTargetCol;
  std::string mEntryText;
  std::string mMarker;

  // Properties of the current menu.
  int mCurMenu;
  GUI::Rect mMatrixRect;
  int mNumMatrixRows,
      mNumMatrixCols;

  // Internal state.
  int mSequenceCount;
  std::vector<int> mSequence;
  std::vector<int>::const_iterator mSequencePos;
  bool mAvoidStimulusRepetition;


  enum
  {
    dontSleep = 0,
    sleep1 = 1,
    sleep2 = 2,

    unsleepAfter

  };
  int mSleepMode;
  bool mPaused;

  std::string mGoalText;
  // Histories for "back" and "undo" functionality.
  std::stack<int>         mMenuHistory;
  std::stack<std::string> mTextHistory;

  // Set of all existing stimuli.
  SetOfStimuli       mStimuli;

  // Display elements.
  StatusBar*         mpStatusBar;
  TextWindow*        mpTextWindow;

  // UDP output.
  sending_udpsocket  mSocket;
  sockstream         mConnection;

 private:
  // Summary log file information.
  LogFile            mSummaryFile;
  std::ostringstream mSelectionSummary;
  int                mRunCount,
                     mNumSelections,
                     mSleepDuration;
  std::time_t        mStartPause;
};

#endif // P3_SPELLER_TASK_H
