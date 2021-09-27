#include "Game.hpp"
#include "Platform.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Utility.hpp"

Game::Game()
{
	m_curLevel = 0;
}

Game::~Game()
{
}

void Game::setup(const sf::Vector2f& winSize)
{
    m_winSize = winSize;

	loadTextures();

	m_bgImg.setTexture(m_bgTexture);
	m_bgImg.setScale(m_winSize.x / m_bgTexture.getSize().x, m_winSize.y / m_bgTexture.getSize().y);

	//loadLevel(1);
}

void Game::reset()
{
	m_over = false;
	m_paused = false;
	m_player.reset();

	removeAllPlatforms();
}

void Game::setPaused(bool pause)
{
	m_paused = pause;
}

bool Game::isPaused() const
{
    return m_paused;
}

bool Game::isOver() const
{
	return m_over;
}

void Game::loadTextures()
{
    m_tile_textures.fill(sf::Texture());

	for (int i = 0; i < 12; i++) {
		m_tile_textures[i].loadFromFile("data/assets/images/Tile (" + std::to_string(i) + ").png");
		m_tile_textures[i].setRepeated(true);
	}
	m_bgTexture.loadFromFile("data/assets/images/BG2.png");

}

void Game::start()
{
	m_curLevel = 1; // TO DO : load from saved file

	loadLevel(m_curLevel);
}

void Game::loadLevel(int level)
{
	reset();

	std::ifstream file;
	file.open("data/assets/Levels/level" + std::to_string(level) + ".txt");

	std::string s;
	while (std::getline(file, s)) {
		std::stringstream line(s);
		char type;
		line >> type;
		if (type == 'l') {
			int w, h;
			line >> w >> h;

			m_lvlSize.x = w * m_tileSize.x;
			m_lvlSize.y = h * m_tileSize.y;

			addPlatform({ 0, 0 }, { 1, h }, m_tileSize, m_tile_textures);
			addPlatform({ w - 1, 0 }, { 1, h }, m_tileSize, m_tile_textures);

		}
		else if (type == '#') {
			int x, y, w, h;
			line >> x >> y >> w >> h;

			addPlatform({ x, y }, { w, h }, m_tileSize, m_tile_textures);
		}
		else if (type == 'p') {
			int x, y;
			line >> x >> y;
			m_player.setPosition(x * m_tileSize.x, y * m_tileSize.y);
		}
	}

	file.close();
}

void Game::loadNextLevel()
{
	loadLevel(
		m_curLevel = (m_curLevel + 1) % (m_totalLevels) + 1
	);
}

void Game::loadSameLevel()
{
	loadLevel(m_curLevel);
}

bool Game::pollEvents(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed){
		if(event.key.code == sf::Keyboard::Space){
			if(m_player.isGrounded())m_player.jump();
		}
	}
	if (event.type == sf::Event::KeyReleased) {

	}
	return false;
}

void Game::update(float dt)
{
   m_player.update(dt);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Vector2f camCenter(m_player.getPosition() + sf::Vector2f(m_player.getLocalBounds().width / 2.f, m_player.getLocalBounds().height / 2.f));

	camCenter.x = std::min(camCenter.x, m_lvlSize.x - m_winSize.x * 0.5f);
	camCenter.y = std::min(camCenter.y, m_lvlSize.y - m_winSize.y * 0.5f);

	camCenter.x = std::max(camCenter.x, m_winSize.x * 0.5f);
	camCenter.y = std::max(camCenter.y, m_winSize.y * 0.5f);

	sf::View camView(camCenter, m_winSize);


	target.draw(m_bgImg, states);

	target.setView(camView);
	
	m_player.draw(target, states);
	drawAllPlatforms(target, states);
	

	/*sf::View miniView;
	miniView.setCenter(m_lvlSize.x * 0.5f, m_lvlSize.y * 0.5f);
	miniView.setSize(m_lvlSize);
	miniView.setViewport(sf::FloatRect(0.75, 0, 0.25, 0.25));

	target.setView(miniView);
	
	target.draw(m_bgImg, states);
	m_player.draw(target, states);
	drawAllPlatforms(target, states);
	*/
	


	target.setView(target.getDefaultView());
}
