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
	class PlayerThrustingSystem : EntityProcessingSystem
	{
		ComponentMapper<Velocity> m_velocityMapper;
		ComponentMapper<Transform> m_transformMapper;
		KeyboardState m_keyboardState;

		public PlayerThrustingSystem()
			: base(typeof(Transform), typeof(Velocity), typeof(PlayerThrusting))
		{
		}

		public override void Initialize()
		{
			m_velocityMapper = new ComponentMapper<Velocity>(world);
			m_transformMapper = new ComponentMapper<Transform>(world);
		}

		public override void Process(Entity e)
		{
			m_keyboardState = Keyboard.GetState();
			float dt = (float)(world.Delta) / 1000.0f;

			Velocity velocity = m_velocityMapper.Get(e);
			Transform transform = m_transformMapper.Get(e);

			Vector2 direction = transform.getDirection();


			Vector2 velocityDifference = Vector2.Zero;


			if (m_keyboardState.IsKeyDown(Keys.W))
				velocityDifference = direction;
			if (m_keyboardState.IsKeyDown(Keys.S))
				velocityDifference = - direction;


			velocity.velocity += velocityDifference * dt * 0.001f;
		}
	}
}
