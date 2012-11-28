using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using Microsoft.Xna.Framework.Input;
using ArtemisAndXNA_tryingOutHeaven.Components;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Systems
{
	class PlayerSteeringSystem : EntityProcessingSystem
	{
		ComponentMapper<Transform> m_transformMapper;
		KeyboardState m_keyboardState;

		public PlayerSteeringSystem()
			: base(typeof(Transform), typeof(PlayerSteering))
		{
		}

		public override void Initialize()
		{
			m_transformMapper = new ComponentMapper<Transform>(world);
		}

		public override void Process(Entity e)
		{
			m_keyboardState = Keyboard.GetState();
			float dt = (float)(world.Delta) / 1000.0f;

			Transform transform = m_transformMapper.Get(e);

			if(m_keyboardState.IsKeyDown(Keys.A))
				transform.rotation -= dt * 4.0f;
			if (m_keyboardState.IsKeyDown(Keys.D))
				transform.rotation += dt * 4.0f;
			

		}
	}
}
