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

	std::ifstream settings;
	settings.open("data/assets/Settings/settings.txt");
	std::string line;
	while (std::getline(settings, line)) {

		std::stringstream ss(line);

		std::string key;
		ss >> key;

		if (key == "TotalLevels") {
			ss >> m_totalLevels;
			break;
		}
	}

	settings.close();

	//loadLevel(1);
}

void Game::reset()
{
	m_over = false;
	m_paused = false;
	m_player.reset();

	removeAllPlatforms();
	removeAllEnemies();
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

	m_allTextures.emplace_back();
	m_allTextures.back().loadFromFile("data/assets/images/zombie.png");
	m_allTextures.emplace_back();
	m_allTextures.back().loadFromFile("data/assets/images/zombie_dead.png");
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
		else if (type == 'r') {
			int x, y, w, h;
			line >> x >> y >> w >> h;
			addRedPlatform({ x,y }, { w,h }, m_tileSize, m_tile_textures);
		}
		else if (type == 'e') {
			int x, y;
			line >> x >> y;
			addEnemy(sf::Vector2f(x * m_tileSize.x, y * m_tileSize.y), m_allTextures[0], m_allTextures[1]);
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

int Game::getTotalLevels()
{
	return m_totalLevels;
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

	sf::Vector2f camCenter(m_player.getPosition() + sf::Vector2f(m_player.getLocalBounds().width / 2.f, m_player.getLocalBounds().height / 2.f));
	camCenter.x = std::min(camCenter.x, m_lvlSize.x - (m_winSize.x * 0.5f));
	camCenter.y = std::min(camCenter.y, m_lvlSize.y - (m_winSize.y * 0.5f));
	camCenter.x = std::max(camCenter.x, m_winSize.x * 0.5f);
	camCenter.y = std::max(camCenter.y, m_winSize.y * 0.5f);

	m_camView.setCenter(camCenter);
	m_camView.setSize(m_winSize);

	//std::cout << camCenter.x - m_winSize.x/2.f << " " << camCenter.y - m_winSize.y/2.f << std::endl;

	Enemy::m_viewBox = sf::FloatRect(camCenter.x - m_winSize.x/2.f, camCenter.y - m_winSize.y/2.f, m_winSize.x, m_winSize.y);
	
	updateAllEnemies(dt);    
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(m_bgImg, states);

	/*sf::Vector2f camCenter(m_player.getPosition() + sf::Vector2f(m_player.getLocalBounds().width / 2.f, m_player.getLocalBounds().height / 2.f));
	camCenter.x = std::min(camCenter.x, m_lvlSize.x - m_winSize.x * 0.5f);
	camCenter.y = std::min(camCenter.y, m_lvlSize.y - m_winSize.y * 0.5f);
	camCenter.x = std::max(camCenter.x, m_winSize.x * 0.5f);
	camCenter.y = std::max(camCenter.y, m_winSize.y * 0.5f);
	std::cout << camCenter.x << " " << camCenter.y << std::endl;
	sf::View camView(camCenter, m_winSize);
	target.setView(camView);*/

	target.setView(m_camView);
	
	m_player.draw(target, states);
	drawAllEnemies(target, states);
	drawAllPlatforms(target, states);
	
	/*sf::RectangleShape rect;
	rect.setSize(m_camView.getSize());
	rect.setOutlineThickness(-2);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOrigin(rect.getSize() / 2.f);
	rect.setPosition(m_camView.getCenter());
	target.draw(rect, states);*/


	//////////////////////////////////////////////////////////////////////
	sf::View miniView;
	miniView.setCenter(m_lvlSize.x * 0.5f, m_lvlSize.y * 0.5f);
	miniView.setSize(m_lvlSize);
	miniView.setViewport(sf::FloatRect(0.75, 0, 0.25, 0.25));
	target.setView(miniView);
	target.draw(m_bgImg, states);
	m_player.draw(target, states);
	drawAllPlatforms(target, states);
	drawAllEnemies(target, states);
	sf::RectangleShape rect;
	rect.setSize(m_camView.getSize());
	rect.setOutlineThickness(-2);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOrigin(rect.getSize() / 2.f);
	rect.setPosition(m_camView.getCenter());
	target.draw(rect, states);
	////////////////////////////////////////////////////////////////

	target.setView(target.getDefaultView());
}

bool Game::isGameOver() {
	return m_player.isPlayerDead();
}