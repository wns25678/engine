/*
 * Copyright (C) 2003, 2009, 2012 Apple Inc. All rights reserved.
 *
 * Portions are Copyright (C) 1998 Netscape Communications Corporation.
 *
 * Other contributors:
 *   Robert O'Callahan <roc+@cs.cmu.edu>
 *   David Baron <dbaron@fas.harvard.edu>
 *   Christian Biesinger <cbiesinger@web.de>
 *   Randall Jesup <rjesup@wgate.com>
 *   Roland Mainz <roland.mainz@informatik.med.uni-giessen.de>
 *   Josh Soref <timeless@mac.com>
 *   Boris Zbarsky <bzbarsky@mit.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alternatively, the contents of this file may be used under the terms
 * of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deletingthe provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#ifndef RenderLayerScrollableArea_h
#define RenderLayerScrollableArea_h

#include "platform/scroll/ScrollableArea.h"

namespace blink {

enum ResizerHitTestType {
    ResizerForPointer,
    ResizerForTouch
};

class PlatformEvent;
class RenderBox;
class RenderLayer;

class RenderLayerScrollableArea final : public ScrollableArea {
public:
    // FIXME: We should pass in the RenderBox but this opens a window
    // for crashers during RenderLayer setup (see crbug.com/368062).
    RenderLayerScrollableArea(RenderLayer&);
    virtual ~RenderLayerScrollableArea();

    bool hasHorizontalScrollbar() const { return horizontalScrollbar(); }
    bool hasVerticalScrollbar() const { return verticalScrollbar(); }

    virtual Scrollbar* horizontalScrollbar() const override { return m_hBar.get(); }
    virtual Scrollbar* verticalScrollbar() const override { return m_vBar.get(); }

    virtual HostWindow* hostWindow() const override;

    virtual GraphicsLayer* layerForScrolling() const override;
    virtual GraphicsLayer* layerForHorizontalScrollbar() const override;
    virtual GraphicsLayer* layerForVerticalScrollbar() const override;
    virtual GraphicsLayer* layerForScrollCorner() const override;
    virtual bool usesCompositedScrolling() const override;
    virtual void invalidateScrollbarRect(Scrollbar*, const IntRect&) override;
    virtual void invalidateScrollCornerRect(const IntRect&) override;
    virtual bool isActive() const override;
    virtual bool isScrollCornerVisible() const override;
    virtual IntRect scrollCornerRect() const override;
    virtual IntRect convertFromScrollbarToContainingView(const Scrollbar*, const IntRect&) const override;
    virtual IntRect convertFromContainingViewToScrollbar(const Scrollbar*, const IntRect&) const override;
    virtual IntPoint convertFromScrollbarToContainingView(const Scrollbar*, const IntPoint&) const override;
    virtual IntPoint convertFromContainingViewToScrollbar(const Scrollbar*, const IntPoint&) const override;
    virtual int scrollSize(ScrollbarOrientation) const override;
    virtual void setScrollOffset(const IntPoint&) override;
    virtual IntPoint scrollPosition() const override;
    virtual IntPoint minimumScrollPosition() const override;
    virtual IntPoint maximumScrollPosition() const override;
    virtual IntRect visibleContentRect(IncludeScrollbarsInRect) const override;
    virtual int visibleHeight() const override;
    virtual int visibleWidth() const override;
    virtual IntSize contentsSize() const override;
    virtual IntSize overhangAmount() const override;
    virtual IntPoint lastKnownMousePosition() const override;
    virtual bool scrollbarsCanBeActive() const override;
    virtual IntRect scrollableAreaBoundingBox() const override;
    virtual bool userInputScrollable(ScrollbarOrientation) const override;
    virtual bool shouldPlaceVerticalScrollbarOnLeft() const override;
    virtual int pageStep(ScrollbarOrientation) const override;

    int scrollXOffset() const { return m_scrollOffset.width() + scrollOrigin().x(); }
    int scrollYOffset() const { return m_scrollOffset.height() + scrollOrigin().y(); }

    IntSize scrollOffset() const { return m_scrollOffset; }

    // FIXME: We shouldn't allow access to m_overflowRect outside this class.
    LayoutRect overflowRect() const { return m_overflowRect; }

    void scrollToOffset(const IntSize& scrollOffset, ScrollOffsetClamping = ScrollOffsetUnclamped);
    void scrollToXOffset(int x, ScrollOffsetClamping clamp = ScrollOffsetUnclamped) { scrollToOffset(IntSize(x, scrollYOffset()), clamp); }
    void scrollToYOffset(int y, ScrollOffsetClamping clamp = ScrollOffsetUnclamped) { scrollToOffset(IntSize(scrollXOffset(), y), clamp); }

    void updateAfterLayout();
    void updateAfterStyleChange(const RenderStyle*);
    void updateAfterOverflowRecalc();

    virtual bool updateAfterCompositingChange() override;

    bool hasScrollbar() const { return m_hBar || m_vBar; }

    void resize(const PlatformEvent&, const LayoutSize&);
    IntSize offsetFromResizeCorner(const IntPoint& absolutePoint) const;

    bool inResizeMode() const { return m_inResizeMode; }
    void setInResizeMode(bool inResizeMode) { m_inResizeMode = inResizeMode; }

    IntRect touchResizerCornerRect(const IntRect& bounds) const
    {
        return resizerCornerRect(bounds, ResizerForTouch);
    }

    LayoutUnit scrollWidth() const;
    LayoutUnit scrollHeight() const;
    int pixelSnappedScrollWidth() const;
    int pixelSnappedScrollHeight() const;

    int verticalScrollbarWidth(OverlayScrollbarSizeRelevancy = IgnoreOverlayScrollbarSize) const;
    int horizontalScrollbarHeight(OverlayScrollbarSizeRelevancy = IgnoreOverlayScrollbarSize) const;

    IntSize adjustedScrollOffset() const { return IntSize(scrollXOffset(), scrollYOffset()); }

    void paintResizer(GraphicsContext*, const IntPoint& paintOffset, const IntRect& damageRect);
    void paintOverflowControls(GraphicsContext*, const IntPoint& paintOffset, const IntRect& damageRect, bool paintingOverlayControls);
    void paintScrollCorner(GraphicsContext*, const IntPoint&, const IntRect& damageRect);

    void positionOverflowControls(const IntSize& offsetFromRoot);

    // isPointInResizeControl() is used for testing if a pointer/touch position is in the resize control
    // area.
    bool isPointInResizeControl(const IntPoint& absolutePoint, ResizerHitTestType) const;
    bool hitTestOverflowControls(HitTestResult&, const IntPoint& localPoint);

    bool hitTestResizerInFragments(const LayerFragments&, const HitTestLocation&) const;

    LayoutRect exposeRect(const LayoutRect&, const ScrollAlignment& alignX, const ScrollAlignment& alignY);

    // Returns true our scrollable area is in the FrameView's collection of scrollable areas. This can
    // only happen if we're both scrollable, and we do in fact overflow. This means that overflow: hidden
    // layers never get added to the FrameView's collection.
    bool scrollsOverflow() const { return m_scrollsOverflow; }

    // Rectangle encompassing the scroll corner and resizer rect.
    IntRect scrollCornerAndResizerRect() const;

    void updateNeedsCompositedScrolling();
    bool needsCompositedScrolling() const { return m_needsCompositedScrolling; }

    // These are used during compositing updates to determine if the overflow
    // controls need to be repositioned in the GraphicsLayer tree.
    void setTopmostScrollChild(RenderLayer*);
    RenderLayer* topmostScrollChild() const { ASSERT(!m_nextTopmostScrollChild); return m_topmostScrollChild; }

private:
    bool hasHorizontalOverflow() const;
    bool hasVerticalOverflow() const;
    bool hasScrollableHorizontalOverflow() const;
    bool hasScrollableVerticalOverflow() const;

    void computeScrollDimensions();

    IntSize clampScrollOffset(const IntSize&) const;

    void setScrollOffset(const IntSize& scrollOffset) { m_scrollOffset = scrollOffset; }

    IntRect rectForHorizontalScrollbar(const IntRect& borderBoxRect) const;
    IntRect rectForVerticalScrollbar(const IntRect& borderBoxRect) const;
    LayoutUnit verticalScrollbarStart(int minX, int maxX) const;
    LayoutUnit horizontalScrollbarStart(int minX) const;
    IntSize scrollbarOffset(const Scrollbar*) const;

    PassRefPtr<Scrollbar> createScrollbar(ScrollbarOrientation);
    void destroyScrollbar(ScrollbarOrientation);

    void setHasHorizontalScrollbar(bool hasScrollbar);
    void setHasVerticalScrollbar(bool hasScrollbar);

    // See comments on isPointInResizeControl.
    IntRect resizerCornerRect(const IntRect&, ResizerHitTestType) const;
    bool overflowControlsIntersectRect(const IntRect& localRect) const;
    void updateResizerAreaSet();
    void drawPlatformResizerImage(GraphicsContext*, IntRect resizerCornerRect);

    RenderBox& box() const;
    RenderLayer* layer() const;

    void updateScrollableAreaSet(bool hasOverflow);

    void updateCompositingLayersAfterScroll();

    RenderLayer& m_layer;

    // Keeps track of whether the layer is currently resizing, so events can cause resizing to start and stop.
    unsigned m_inResizeMode : 1;
    unsigned m_scrollsOverflow : 1;

    unsigned m_scrollDimensionsDirty : 1;
    unsigned m_inOverflowRelayout : 1;

    RenderLayer* m_nextTopmostScrollChild;
    RenderLayer* m_topmostScrollChild;

    // FIXME: once cc can handle composited scrolling with clip paths, we will
    // no longer need this bit.
    unsigned m_needsCompositedScrolling : 1;

    // The width/height of our scrolled area.
    LayoutRect m_overflowRect;

    // This is the (scroll) offset from scrollOrigin().
    IntSize m_scrollOffset;

    IntPoint m_cachedOverlayScrollbarOffset;

    // For areas with overflow, we have a pair of scrollbars.
    RefPtr<Scrollbar> m_hBar;
    RefPtr<Scrollbar> m_vBar;
};

} // namespace blink

#endif // RenderLayerScrollableArea_h
