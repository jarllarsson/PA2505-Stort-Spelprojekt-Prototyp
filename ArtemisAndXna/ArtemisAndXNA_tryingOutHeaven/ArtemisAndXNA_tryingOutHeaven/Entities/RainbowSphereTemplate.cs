using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Components;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ArtemisAndXNA_tryingOutHeaven.Entities
{
	class RainbowSphereTemplate : IEntityTemplate
	{

		public Entity BuildEntity(Entity e, params object[] args)
		{
			e.AddComponent<Transform>(new Transform(
				(float)args[0],
				(float)args[1],
				0.1f, 0.1f));
			e.AddComponent<Velocity>(new Velocity(
				(float)args[2],
				(float)args[3]));
			e.AddComponent<Render>(new Render(
				(Texture2D)args[4], Color.White));

			return e;
		}
	}
}
