#pragma once

#include "ZUIFrame.hpp"
#include <string>

namespace zui {
class Textbox : public Entity
{
protected:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor for Children Classes
    /// 
    /// \param class_id -> ZUI Class Id of the child class
    ///
    ////////////////////////////////////////////////////////////
    Textbox(const sf::Vector2f size, int class_id);

public:
    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the text alignment positions
    ///
    ////////////////////////////////////////////////////////////
    enum class TextAlign { LEFT, CENTER, RIGHT };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param size -> size of the box
    /// 
    ////////////////////////////////////////////////////////////
    Textbox(const sf::Vector2f& size = sf::Vector2f(0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Virtual Default Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Textbox();

    ////////////////////////////////////////////////////////////
    /// \brief Set the size of the box
    ///
    /// \param size -> New size
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSize(const sf::Vector2f& size);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the box
    ///
    /// \return size of the box
    ///
    ////////////////////////////////////////////////////////////
    const sf::Vector2f& getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's string
    ///
    /// \param string -> New string
    ///
    ////////////////////////////////////////////////////////////
    void setString(const std::string& string);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's font
    ///
    /// The \a font argument refers to a font that must
    /// exist as long as the text uses it. Indeed, the text
    /// doesn't store its own copy of the font, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the font is destroyed and the text tries to
    /// use it, the behavior is undefined.
    ///
    /// \param font -> New font
    ///
    ////////////////////////////////////////////////////////////
    void setFont(const sf::Font& font);

    ////////////////////////////////////////////////////////////
    /// \brief Set the character size
    ///
    /// The default size is 30.
    ///
    /// Note that if the used font is a bitmap font, it is not
    /// scalable, thus not all requested sizes will be available
    /// to use. This needs to be taken into consideration when
    /// setting the character size. If you need to display text
    /// of a certain size, make sure the corresponding bitmap
    /// font that supports that size is used.
    ///
    /// \param size -> New character size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void setCharacterSize(uint64_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the line spacing factor
    ///
    /// The default spacing between lines is defined by the font.
    /// This method enables you to set a factor for the spacing
    /// between lines. By default the line spacing factor is 1.
    ///
    /// \param spacingFactor -> New line spacing factor
    ///
    ////////////////////////////////////////////////////////////
    void setLineSpacing(float spacingFactor);

    ////////////////////////////////////////////////////////////
    /// \brief Set the letter spacing factor
    ///
    /// The default spacing between letters is defined by the font.
    /// This factor doesn't directly apply to the existing
    /// spacing between each character, it rather adds a fixed
    /// space between them which is calculated from the font
    /// metrics and the character size.
    /// Note that factors below 1 (including negative numbers) bring
    /// characters closer to each other.
    /// By default the letter spacing factor is 1.
    ///
    /// \param spacingFactor -> New letter spacing factor
    ///
    ////////////////////////////////////////////////////////////
    void setLetterSpacing(float spacingFactor);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's style
    ///
    /// You can pass a combination of one or more styles, for
    /// example sf::Text::Bold | sf::Text::Italic.
    /// The default style is sf::Text::Regular.
    ///
    /// \param style -> New style
    ///
    ////////////////////////////////////////////////////////////
    void setStyle(sf::Uint32 style);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the box
    ///
    /// By default, the box's fill color is opaque white
    /// 
    /// \param color -> New fill color of the box
    ///
    ////////////////////////////////////////////////////////////
    virtual void setFillColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the outline color of the box
    ///
    /// By default, the box's outline color is White.
    ///
    /// \param color -> New outline color of the box
    ///
    ////////////////////////////////////////////////////////////
    virtual void setOutlineColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the box's outline
    ///
    /// By default, the outline thickness is 0.
    ///
    /// Be aware that using a negative value for the outline
    /// thickness will cause distorted rendering.
    ///
    /// \param thickness -> New outline thickness, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setOutlineThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Change the source texture of the shape
    ///
    /// The \a texture argument refers to a texture that must
    /// exist as long as the shape uses it. Indeed, the shape
    /// doesn't store its own copy of the texture, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source texture is destroyed and the shape tries to
    /// use it, the behavior is undefined.
    /// \a texture can be NULL to disable texturing.
    /// If \a resetRect is true, the TextureRect property of
    /// the shape is automatically adjusted to the size of the new
    /// texture. If it is false, the texture rect is left unchanged.
    ///
    /// \param texture   New texture
    /// \param resetRect Should the texture rect be reset to the size of the new texture?
    ///
    ////////////////////////////////////////////////////////////
    void setTexture(const sf::Texture* texture, bool resetRect = false);

    ////////////////////////////////////////////////////////////
    /// \brief Set the sub-rectangle of the texture that the shape will display
    ///
    /// The texture rect is useful when you don't want to display
    /// the whole texture, but rather a part of it.
    /// By default, the texture rect covers the entire texture.
    ///
    /// \param rect Rectangle defining the region of the texture to display
    ///
    ////////////////////////////////////////////////////////////
    void setTextureRect(const sf::IntRect& rect);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the text
    ///
    /// By default, the text's fill color is opaque white
    ///
    /// \param color -> New fill color of the text
    ///
    ////////////////////////////////////////////////////////////
    void setTextFillColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the outline color of the text
    ///
    /// By default, the text's outline color is opaque black.
    ///
    /// \param color -> New outline color of the text
    ///
    ////////////////////////////////////////////////////////////
    void setTextOutlineColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the text's outline
    ///
    /// By default, the outline thickness is 0.
    ///
    /// Be aware that using a negative value for the outline
    /// thickness will cause distorted rendering.
    ///
    /// \param thickness -> New outline thickness, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void setTextOutlineThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's string
    ///
    /// \return Text's string
    ///
    ////////////////////////////////////////////////////////////
    std::string getString() const;
    
    ////////////////////////////////////////////////////////////
    /// \brief Clear the text's string
    ///
    ////////////////////////////////////////////////////////////
    void clearString();
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the text's font
    ///
    /// If the text has no font attached, a NULL pointer is returned.
    /// The returned pointer is const, which means that you
    /// cannot modify the font when you get it from this function.
    ///
    /// \return Pointer to the text's font
    ///
    ////////////////////////////////////////////////////////////
    const sf::Font* getFont() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the character size
    ///
    /// \return Size of the characters, in pixels
    ///
    ////////////////////////////////////////////////////////////
    uint64_t getCharacterSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the letter spacing factor
    ///
    /// \return Size of the letter spacing factor
    ///
    ////////////////////////////////////////////////////////////
    float getLetterSpacing() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the line spacing factor
    ///
    /// \return Size of the line spacing factor
    ///
    ////////////////////////////////////////////////////////////
    float getLineSpacing() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's style
    ///
    /// \return Text's style
    ///
    ////////////////////////////////////////////////////////////
    sf::Uint32 getStyle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the box
    ///
    /// \return Fill color of the box
    ///
    ////////////////////////////////////////////////////////////

    const sf::Color& getFillColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline color of the box
    ///
    /// \return Outline color of the box
    ///
    ////////////////////////////////////////////////////////////
    const sf::Color& getOutlineColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline thickness of the box
    ///
    /// \return Outline thickness of the box, in pixels
    ///
    ////////////////////////////////////////////////////////////
    float getOutlineThickness() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the source texture of the shape
    ///
    /// If the shape has no source texture, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the texture when you retrieve it with this function.
    ///
    /// \return Pointer to the shape's texture
    ///
    ////////////////////////////////////////////////////////////
    const sf::Texture* getTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sub-rectangle of the texture displayed by the shape
    ///
    /// \return Texture rectangle of the shape
    ///
    ////////////////////////////////////////////////////////////
    const sf::IntRect& getTextureRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the text
    ///
    /// \return Fill color of the text
    ///
    ////////////////////////////////////////////////////////////
    const sf::Color& getTextFillColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline color of the text
    ///
    /// \return Outline color of the text
    ///
    ////////////////////////////////////////////////////////////
    const sf::Color& getTextOutlineColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline thickness of the text
    ///
    /// \return Outline thickness of the text, in pixels
    ///
    ////////////////////////////////////////////////////////////
    float getTextOutlineThickness() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the position of the \a index-th character
    ///
    /// This function computes the visual position of a character
    /// from its index in the string. The returned position is
    /// in global coordinates (translation, rotation, scale and
    /// origin are applied).
    /// If \a index is out of range, the position of the end of
    /// the string is returned.
    ///
    /// \param index -> Index of the character
    ///
    /// \return Position of the character
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f findCharacterPos(std::size_t index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the local bounding rectangle of the entity
    ///
    /// The returned rectangle is in local coordinates, which means
    /// that it ignores the transformations (translation, rotation,
    /// scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// entity in the entity's coordinate system.
    ///
    /// \return Local bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    sf::FloatRect getLocalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// text in the global 2D world's coordinate system.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    sf::FloatRect getGlobalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the alignment of the text with the box
    /// 
    /// ////////////////////////////////////////////////////////////
    void alignText(TextAlign textAlign = TextAlign::CENTER);

    ////////////////////////////////////////////////////////////
    /// \brief Check if the point is inside the object
    ///		   Definition of if the point is inside the object may vary with different zui objects
    /// 
    /// \return true if point is inside the object, false otherwise
    /// 
    /// \param point -> Position of the point
    /// 
    ////////////////////////////////////////////////////////////
    virtual bool contains(const sf::Vector2f& point) const override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the entity if the point is over the entity
    /// 
    /// \return Pointer to the entity if pointed by the point, nullptr otherwise
    /// 
    /// \param point -> Position of the point
    /// 
    ////////////////////////////////////////////////////////////
    virtual Entity* isHit(const sf::Vector2f& point) override;

    ////////////////////////////////////////////////////////////
    /// \brief Actions to take upon selection of the object
    ///
    ////////////////////////////////////////////////////////////
    virtual void activateSelection() override;

    ////////////////////////////////////////////////////////////
    /// \brief Actions to take upon deselection of the object
    ///
    ////////////////////////////////////////////////////////////
    virtual void deactivateSelection() override;

    ////////////////////////////////////////////////////////////
    /// \brief Draws the object to the target applying its states
    /// 
    /// \param target -> Render target to draw to
    /// \param states -> Current render states
    /// 
    ////////////////////////////////////////////////////////////
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;


    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
protected:
	sf::RectangleShape m_box;       /// < box object of textbox
    mutable sf::Text m_text;        /// < text object of textbox, mutable for cursor updating
    TextAlign m_alignment;          /// < alignment of text with the box
};


typedef std::unique_ptr<zui::Textbox> Textbox_ptr;


} // namespace zui
