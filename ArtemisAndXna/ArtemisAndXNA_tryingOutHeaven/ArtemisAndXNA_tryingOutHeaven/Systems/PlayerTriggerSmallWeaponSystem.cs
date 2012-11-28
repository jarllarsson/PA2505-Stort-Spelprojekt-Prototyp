using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Components;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Systems
{
	class PlayerTriggerSmallWeaponSystem : EntityProcessingSystem
	{
		ComponentMapper<Transform> m_transformMapper;
		ComponentMapper<Velocity> m_velocityMapper;
		ComponentMapper<PlayerTriggeringSmallWeapon> m_weaponMapper;

		Dictionary<string, Texture2D> m_textures;

		public PlayerTriggerSmallWeaponSystem(Dictionary<string, Texture2D> p_textures)
			: base(typeof(PlayerTriggeringSmallWeapon))
		{
			m_textures = p_textures;
		}

		public override void Initialize()
		{
			m_transformMapper = new ComponentMapper<Transform>(world);
			m_velocityMapper = new ComponentMapper<Velocity>(world);
			m_weaponMapper = new ComponentMapper<PlayerTriggeringSmallWeapon>(world);

		}

		public override void Process(Entity e)
		{
			PlayerTriggeringSmallWeapon weapon =
				m_weaponMapper.Get(e);
			Transform transform = m_transformMapper.Get(e);
			Velocity velocity = m_velocityMapper.Get(e);

			float dt = (float)(world.Delta) / 1000.0f;

			weapon.cooldown -= dt;

			if (weapon.cooldown < 0)
				weapon.cooldown = 0;

			if (Keyboard.GetState().IsKeyDown(Keys.Space))
			{
				if (weapon.cooldown <= 0)
				{
					Vector2 direction = transform.getDirection();
					Vector2 right = new Vector2(-direction.Y, direction.X);

					direction *= 0.001f;

					Vector2 sphereVelocity =
						velocity.velocity + direction;

					Vector2 spherePosition =
						transform.position + right * 65.0f;

					// Right weapon.
					Entity entity = world.CreateEntity("rainbow_sphere",
						spherePosition.X,
						spherePosition.Y,
						sphereVelocity.X,
						sphereVelocity.Y,
						m_textures["rainbow_sphere"]);
					entity.Refresh();

					spherePosition =
						transform.position - right * 65.0f;

					// Left weapon.
					entity = world.CreateEntity("rainbow_sphere",
						spherePosition.X,
						spherePosition.Y,
						sphereVelocity.X,
						sphereVelocity.Y,
						m_textures["rainbow_sphere"]);
					entity.Refresh();

					weapon.cooldown = 0.05f;
				}
			}
		}
	}
}
