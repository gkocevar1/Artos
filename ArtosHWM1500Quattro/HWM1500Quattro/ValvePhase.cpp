#include "ValvePhase.h"

// ------------------
// Public methods

/**
* Switch valves to desired phase
*/
void ValvePhase::switchToPhase(Constants::Phase phase)
{
  DMSG1("New Phase: ");DMSG1(phase);DMSG1(" ");
	DMSG(Constants::PhaseNames[phase]);

	switch (phase)
	{
  	case Constants::Phase::FlushingSFRusco:
  	{
  		ValvePhase::switchValves(
  			false, // 1
  			false, // 2
  			false, // 3
  			false, // 4
  			false, // 5
  			true, // 6
  			false, // 7
  			false, // 8
  			false, // 9
  			false, // 10
  			false, // 11
  			false, // 12
  			false, // 13
  			false  // 14
  			);
  		break;
  	}
  	case Constants::Phase::FlushingUF1B:
    case Constants::Phase::FlushingUF2G:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			true, // 3
  			true, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			false, // 9
  			true, // 10
  			true, // 11
  			true, // 12
  			true, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::FlushingGAC:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			false, // 3
  			false, // 4
  			true, // 5
  			false, // 6
  			true, // 7
  			true, // 8
  			false, // 9
  			true, // 10
  			true, // 11
  			false, // 12
  			false, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::Filtration:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			false, // 3
  			false, // 4
  			true, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			true, // 10
  			true, // 11
  			false, // 12
  			false, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::BackwashRusco:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			false, // 3
  			false, // 4
  			true, // 5
  			true, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			true, // 10
  			true, // 11
  			false, // 12
  			false, // 13
  			false  // 14
  			);
  		break;
  	}
  	case Constants::Phase::BackwashUF1:
  	{
  		ValvePhase::switchValves(
  			false, // 1
  			true, // 2
  			true, // 3
  			false, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			true, // 10
  			true, // 11
  			false, // 12
  			false, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::BackwashUF2:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			false, // 2
  			false, // 3
  			true, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			true, // 10
  			true, // 11
  			false, // 12
  			false, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::BackwashUF3:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			false, // 3
  			false, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			false, // 10
  			true, // 11
  			true, // 12
  			false, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::BackwashUF4:
  	{
  		ValvePhase::switchValves(
  			true, // 1
  			true, // 2
  			false, // 3
  			false, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			true, // 9
  			true, // 10
  			false, // 11
  			false, // 12
  			true, // 13
  			true  // 14
  			);
  		break;
  	}
  	case Constants::Phase::Desinfection:
  	{
  		ValvePhase::switchValves(
  			false, // 1
  			false, // 2
  			true, // 3
  			true, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			true, // 8
  			true, // 9
  			false, // 10
  			false, // 11
  			true, // 12
  			true, // 13
  			false  // 14
  			);
  		break;
  	}
    case Constants::Phase::FlushingUF2B:
    {
      ValvePhase::switchValves(
        true, // 1
        true, // 2
        true, // 3
        true, // 4
        false, // 5
        false, // 6
        false, // 7
        false, // 8
        true, // 9
        true, // 10
        true, // 11
        true, // 12
        true, // 13
        true  // 14
        );
      break;
    }
  	/*case Constants::Phase::Close:
  	{
  		ValvePhase::switchValves(
  			false, // 1
  			false, // 2
  			false, // 3
  			false, // 4
  			false, // 5
  			false, // 6
  			false, // 7
  			false, // 8
  			false, // 9
  			false, // 10
  			false, // 11
  			false, // 12
  			false, // 13
  			false  // 14
  			);
  		break;
  	}*/
	}
}

// ------------------
// private methods

/**
* switch valves to proper position
*/
void ValvePhase::switchValves(boolean v1, boolean v2, boolean v3, boolean v4, boolean v5, boolean v6, boolean v7, boolean v8, boolean v9, boolean v10, boolean v11, boolean v12, boolean v13, boolean v14)
{
	String info = String(
		"v1:" + ValvePhase::info(v1) + ", " +
		"v2:" + ValvePhase::info(v2) + ", " +
		"v3:" + ValvePhase::info(v3) + ", " +
		"v4:" + ValvePhase::info(v4) + ", " +
		"v5:" + ValvePhase::info(v5) + ", " +
		"v6:" + ValvePhase::info(v6) + ", " +
		"v7:" + ValvePhase::info(v7) + ", " +
		"v8:" + ValvePhase::info(v8) + ", " +
		"v9:" + ValvePhase::info(v9) + ", " +
		"v10:" + ValvePhase::info(v10) + ", " +
		"v11:" + ValvePhase::info(v11) + ", " +
		"v12:" + ValvePhase::info(v12) + ", " +
		"v13:" + ValvePhase::info(v13) + ", " +
		"v14:" + ValvePhase::info(v14));
	DMSG(info);

	if (v1)
	{

	}
	else
	{

	}

	if (v2)
	{

	}
	else
	{

	}

	if (v3)
	{

	}
	else
	{

	}

	if (v4)
	{

	}
	else
	{

	}

	if (v5)
	{

	}
	else
	{

	}

	if (v6)
	{

	}
	else
	{

	}

	if (v7)
	{

	}
	else
	{

	}

	if (v8)
	{

	}
	else
	{

	}

	if (v9)
	{

	}
	else
	{

	}

	if (v10)
	{

	}
	else
	{

	}

	if (v11)
	{

	}
	else
	{

	}

	if (v12)
	{

	}
	else
	{

	}

	if (v13)
	{

	}
	else
	{

	}

	if (v14)
	{

	}
	else
	{

	}
}

/**
* Only to build info string - not needed in PRODUCTION
*/
String ValvePhase::info(boolean v)
{
	return v ? "open" : "close";
}
