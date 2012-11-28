using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Components;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Systems
{
	class DrawSpritesSystem : EntityProcessingSystem
	{
		ComponentMapper<Render> m_renderMapper;
		ComponentMapper<Transform> m_transformMapper;

		Dictionary<string, Texture2D> m_textures;
		SpriteBatch m_spriteBatch;

		public DrawSpritesSystem(Dictionary<string, Texture2D> p_textures,
			SpriteBatch p_spriteBatch )
			: base(typeof(Render), typeof(Transform))
		{
			m_textures = p_textures;
			m_spriteBatch = p_spriteBatch;
		}

		public override void Initialize()
		{
			m_renderMapper = new ComponentMapper<Render>(world);
			m_transformMapper = new ComponentMapper<Transform>(world);
		}

		public override void Process(Entity e)
		{
			Transform transform = m_transformMapper.Get(e);
			Render renderData = m_renderMapper.Get(e);

			Vector2 origin = new Vector2(renderData.texture.Width / 2.0f,
				renderData.texture.Height / 2.0f);

			m_spriteBatch.Draw(renderData.texture,
				transform.position,
				null,
				renderData.color,
				transform.rotation,
				origin,
				transform.scale,
				SpriteEffects.None,
				0.0f);

		}

	}
}
