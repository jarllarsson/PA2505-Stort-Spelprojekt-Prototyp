using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;

namespace ArtemisAndXNA_tryingOutHeaven.Components
{
	class WorldInfo : Component
	{
		public int numberOfEntities;

		public WorldInfo(int p_numberOfEntities)
		{
			numberOfEntities = p_numberOfEntities;
		}
	}
}
