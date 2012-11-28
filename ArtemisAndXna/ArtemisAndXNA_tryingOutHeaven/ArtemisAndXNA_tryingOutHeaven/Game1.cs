using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Systems;
using ArtemisAndXNA_tryingOutHeaven.Components;
using ArtemisAndXNA_tryingOutHeaven.Entities;

namespace ArtemisAndXNA_tryingOutHeaven
{
	/// <summary>
	/// This is the main type for your game
	/// </summary>
	public class Game1 : Microsoft.Xna.Framework.Game
	{
		GraphicsDeviceManager graphics;
		SpriteBatch spriteBatch;

		Dictionary<string, Texture2D> m_textures;
		SpriteFont m_arialFont;

		EntityWorld m_world;

		public Game1()
		{
			graphics = new GraphicsDeviceManager(this);
			Content.RootDirectory = "Content";
		}

		/// <summary>
		/// Allows the game to perform any initialization it needs to before starting to run.
		/// This is where it can query for any required services and load any non-graphic
		/// related content.  Calling base.Initialize will enumerate through any components
		/// and initialize them as well.
		/// </summary>
		protected override void Initialize()
		{
			m_textures = new Dictionary<string, Texture2D>();

			graphics.PreferredBackBufferHeight = 1050;
			graphics.PreferredBackBufferWidth = 1680;
			graphics.ApplyChanges();

			

			base.Initialize();
		}

		/// <summary>
		/// LoadContent will be called once per game and is the place to load
		/// all of your content.
		/// </summary>
		protected override void LoadContent()
		{
			// Create a new SpriteBatch, which can be used to draw textures.
			spriteBatch = new SpriteBatch(GraphicsDevice);

			// Textures:
			m_textures.Add("boss_ship",
				Content.Load<Texture2D>("boss_ship"));

			m_textures.Add("rainbow_sphere",
				Content.Load<Texture2D>("rainbow_sphere"));



			m_arialFont = Content.Load<SpriteFont>("Arial");


			m_world = new EntityWorld();

			// Set templates:
			m_world.SetEntityTemplate("rainbow_sphere", new RainbowSphereTemplate());

			// Systems:
			m_world.SystemManager.SetSystem<PlayerThrustingSystem>(
				new PlayerThrustingSystem(),
				ExecutionType.Update);

			m_world.SystemManager.SetSystem<PlayerSteeringSystem>(
				new PlayerSteeringSystem(),
				ExecutionType.Update);

			m_world.SystemManager.SetSystem<PlayerTriggerSmallWeaponSystem>(
				new PlayerTriggerSmallWeaponSystem(m_textures),
				ExecutionType.Update);

			m_world.SystemManager.SetSystem<MovementSystem>(
				new MovementSystem(),
				ExecutionType.Update);

			m_world.SystemManager.SetSystem<WrapEntitiesOnBoundrySystem>(
				new WrapEntitiesOnBoundrySystem(),
				ExecutionType.Update);

			m_world.SystemManager.SetSystem<DrawSpritesSystem>(
				new DrawSpritesSystem(m_textures, spriteBatch),
				ExecutionType.Draw);

			foreach(EntityProcessingSystem system in m_world.SystemManager.Systems)
			System.Diagnostics.Debug.WriteLine(system.GetType());

			m_world.SystemManager.InitializeAll();

			// Entities:
			Entity e = m_world.CreateEntity();
			e.AddComponent<Transform>(new Transform(840.0f, 400.0f,
				- (float)Math.PI / 2.0f));
			e.AddComponent<Velocity>(new Velocity(0, 0));
			e.AddComponent<Render>(new Render(m_textures["boss_ship"], Color.White));
			e.AddComponent<PlayerSteering>(new PlayerSteering(
				PlayerSteeringType.USING_KEYBOARD));
			e.AddComponent<PlayerThrusting>(new PlayerThrusting());
			e.AddComponent<PlayerTriggeringSmallWeapon>(
				new PlayerTriggeringSmallWeapon());
			e.Refresh();


			e = m_world.CreateEntity("rainbow_sphere",
				400.0f, 400.0f,
				0.0f, 0.0f,
				m_textures["rainbow_sphere"]);
			e.Refresh();

			//e = m_world.CreateEntity();
			//e.AddComponent<Transform>(new Transform(400.0f, 400.0f,
			//	0.1f, 0.1f));
			//e.AddComponent<Velocity>(new Velocity(0, 0));
			//e.AddComponent<Render>(new Render(m_textures["rainbow_sphere"], Color.White));
			//e.Refresh();


		}

		/// <summary>
		/// UnloadContent will be called once per game and is the place to unload
		/// all content.
		/// </summary>
		protected override void UnloadContent()
		{
			// TODO: Unload any non ContentManager content here
		}

		/// <summary>
		/// Allows the game to run logic such as updating the world,
		/// checking for collisions, gathering input, and playing audio.
		/// </summary>
		/// <param name="gameTime">Provides a snapshot of timing values.</param>
		protected override void Update(GameTime gameTime)
		{
			// Allows the game to exit
			if (Keyboard.GetState().IsKeyDown(Keys.Escape))
				this.Exit();

			m_world.LoopStart();
			m_world.Delta = gameTime.ElapsedGameTime.Milliseconds;

			m_world.SystemManager.UpdateSynchronous(ExecutionType.Update);

			base.Update(gameTime);
		}

		/// <summary>
		/// This is called when the game should draw itself.
		/// </summary>
		/// <param name="gameTime">Provides a snapshot of timing values.</param>
		protected override void Draw(GameTime gameTime)
		{
			GraphicsDevice.Clear(Color.CornflowerBlue);

			spriteBatch.Begin(SpriteSortMode.BackToFront, BlendState.AlphaBlend);


			m_world.SystemManager.UpdateSynchronous(ExecutionType.Draw);



			string numEntities =
				"Entities total: " +
				m_world.EntityManager.ActiveEntitiesCount.ToString();

			spriteBatch.DrawString(m_arialFont, numEntities,
				new Vector2(100.0f, 100.0f),
				Color.White, 0,
				Vector2.Zero,
				new Vector2(0.75f, 0.75f),
				SpriteEffects.None, 0.0f);


			spriteBatch.End();

			base.Draw(gameTime);
		}
	}
}
