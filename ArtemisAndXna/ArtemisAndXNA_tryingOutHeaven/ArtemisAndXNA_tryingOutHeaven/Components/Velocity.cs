using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Components
{
	class Velocity : Component
	{
		public Vector2 velocity;

		public Velocity(float p_xVelocity, float p_yVelocity)
		{
			velocity.X = p_xVelocity;
			velocity.Y = p_yVelocity;
		}
	}
}
