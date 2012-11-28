using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Components
{
	class Render : Component
	{
		public Texture2D texture;
		public Color color;

		public Render(Texture2D p_texture, Color p_color)
		{
			texture = p_texture;
			color = p_color;
		}
	}
}
