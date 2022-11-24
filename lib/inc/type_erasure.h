//---- <Shape.h> ----------------

#include <memory>
#include <utility>

namespace detail {

    class ShapeConcept
    {
    public:
        virtual ~ShapeConcept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };


template< typename ShapeT
        , typename DrawStrategy >
    class OwningShapeModel : public ShapeConcept
    {
    public:
        explicit OwningShapeModel(ShapeT shape, DrawStrategy drawer)
            : shape_{ std::move(shape) }
            , drawer_{ std::move(drawer) }
        {}

        void draw() const override { drawer_(shape_); }

        std::unique_ptr<ShapeConcept> clone() const override
        {
            return std::make_unique<OwningShapeModel>(*this);
        }

    private:
        ShapeT shape_;
        DrawStrategy drawer_;
    };

} // namespace detail


class Shape
{
public:
    template< typename ShapeT
            , typename DrawStrategy >
    Shape(ShapeT shape, DrawStrategy drawer)
        : pimpl_ { std::make_unique<detail::OwningShapeModel<
                    ShapeT, DrawStrategy>>(std::move(shape)
                                         , std::move(drawer)) }
    {
    }

    Shape(Shape const& other)
        : pimpl_(other.pimpl_->clone())
    {}

    Shape& operator=(Shape const& other)
    {
        other.pimpl_->clone().swap(pimpl_);
        return *this;
    }

    Shape(Shape&&) = default;
    Shape& operator=(Shape&&) = default;
    
    ~Shape() = default;

    
    // ...

private:
    // ...
    friend void draw(Shape const& shape)
    {
        shape.pimpl_->draw();
    }

     std::unique_ptr<detail::ShapeConcept> pimpl_;
};

