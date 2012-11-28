using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Components
{
	class Transform : Component
	{
		public Vector2 position;
		public Vector2 scale;
		public float rotation;

		public Transform(float p_xPos, float p_yPos)
		{
			position.X = p_xPos;
			position.Y = p_yPos;

			scale = new Vector2(1.0f, 1.0f);

			rotation = 0.0f;
		}

		public Transform(float p_xPos, float p_yPos, float p_rotation)
		{
			position.X = p_xPos;
			position.Y = p_yPos;

			rotation = p_rotation;

			scale = new Vector2(1.0f, 1.0f);
		}

		public Transform(float p_xPos, float p_yPos, float p_xScale, float p_yScale)
		{
			position = new Vector2(p_xPos, p_yPos);

			scale = new Vector2(p_xScale, p_yScale);

			rotation = 0.0f;
		}

		public Vector2 getDirection()
		{
			Vector2 direction = new Vector2(
				(float)Math.Cos(rotation),
				(float)Math.Sin(rotation));

			return direction;
		}

	}
}
