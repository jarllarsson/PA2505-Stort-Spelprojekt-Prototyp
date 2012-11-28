using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;


namespace ArtemisAndXNA_tryingOutHeaven.Components
{
	enum PlayerSteeringType
	{
		USING_KEYBOARD,
		USING_MOUSE,
	}

	class PlayerSteering : Component
	{
		public PlayerSteeringType steeringType;

		public PlayerSteering(PlayerSteeringType p_steeringType)
		{
			steeringType = p_steeringType;
		}
	}
}
