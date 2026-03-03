Simple Timeline
===============

This module provides Views style plugins for rendering content as a vertical timeline. It works with any content type and requires only the Views module.

Important Information
---------------------

The 1.x-2.x release should be fully backward compatible. Existing users of the original Simple Timeline plugin can upgrade without any changes to their views or configuration. The Enhanced Timeline plugin is opt-in — users who want the new features can switch to it at their own pace.

History
-------

Simple Timeline was originally written for Drupal 7 by [Alan Lobo](https://github.com/alansaviolobo) and later ported to Backdrop CMS by [Robert Garrigos](https://github.com/robertgarrigos). That original release (1.x-1.x) provided a single Views style plugin — "Simple Timeline" — along with a custom row plugin called "Simple Timeline Item" that allowed users to map fields to date, text, and image slots on the timeline.

[Tim Erickson](https://github.com/stpaultim) was working on a new implementation featuring proportional time-based spacing, collision avoidance, alternating layouts, mobile responsiveness, and a modern visual design. Rather than releasing it as a separate module, the decision was made to merge it into Simple Timeline as a second Views style plugin called "Enhanced Timeline," releasing the combined work as the 1.x-2.x series.

Views Style Plugins
-------------------

This module provides two Views style plugins:

### Simple Timeline (original)

The original plugin. Renders a chronological list of items in a simple alternating left/right layout. Requires the "Simple Timeline Item" row plugin, which lets you map any field in your view to the date, text, or image area of each timeline card.

To use it:

- Create or edit a view.
- Under **Format**, set the style to **Simple Timeline**.
- Under **Format › Show**, set the row plugin to **Simple Timeline Item**.
- Configure which fields appear in the date, text, and image areas.
- Add a sort by your date field (ascending).
- Save the view.

You can also use the **Content** (node view) row plugin instead of Simple Timeline Item if you prefer rendered entity output.

### Enhanced Timeline (new in 2.x)

A fully redesigned style plugin that positions events proportionally along a vertical axis based on real time gaps between dates. Cards are placed in alternating left/right columns with a connecting dot and stem on the center line. Hover effects highlight the full path from card to dot.

Key features:

- **Date source:** use the node created date (no extra configuration needed) or any date field on your content type.
- **Proportional or equal spacing:** proportional spacing reflects real time between events; equal spacing stacks cards with a consistent gap.
- **Layout modes:** alternating left/right, all on the right, or all on the left.
- **Collision avoidance:** when events are close together in time, cards are nudged apart to prevent overlap, with a vertical stem connecting each card back to its true position on the timeline.
- **Mobile responsive:** below 640px the layout automatically collapses to a single column.
- **Works with any row plugin:** use Content (teaser or full), Fields, or any other standard Views row plugin.

To use it:

- Create or edit a view.
- Under **Format**, set the style to **Enhanced Timeline**.
- Under **Format › Show**, set the row plugin to **Content** (teaser mode recommended) or **Fields**.
- Under **Format**, configure the date source, layout, and spacing options.
- Add a sort by your date field (ascending).
- Save the view.

**Note on pagers:** pagers are supported but not recommended when using proportional spacing. Each page of results recalculates its own minimum and maximum timestamps, which means the time scale shifts between pages and the proportional distances become misleading. If you need a pager, switch to equal spacing.

Demo Content Submodule
----------------------

The `simple_timeline_demo` submodule ships inside the `simple_timeline` directory and is designed to help you get up and running quickly. It is entirely optional and safe to ignore if you are building your own timeline.

**What it installs:**

- A **Timeline Event** content type with a required **Date** field (`field_timeline_date`). The content type is configured to hide the author and submission date by default.
- A **Views page** at `/simple-timeline-demo` using the Enhanced Timeline plugin, sorted by event date, with a link added to the main menu.
- **20 sample nodes** themed around the events of *The Lord of the Rings*, placed on Earth calendar dates spanning 2001 to 2020. These are useful for seeing how proportional spacing behaves across a range of time gaps.
- A status message on install with a direct link to the demo view.

**What happens when you uninstall it:**

Uninstalling `simple_timeline_demo` removes everything it created:

- All 20 sample Timeline Event nodes are deleted.
- The `field_timeline_date` field instance is removed from the Timeline Event bundle. If this module created the field base, the base is also purged; if the field already existed before installation (e.g. because another module created it), the base is left untouched.
- The Timeline Event content type is deleted.
- The demo view configuration (`views.view.simple_timeline_demo`) is deleted.

Nothing from the parent `simple_timeline` module is affected by uninstalling the demo submodule.

Documentation
-------------

Additional documentation is located in the Wiki:
https://github.com/backdrop-contrib/simple_timeline/wiki/Documentation.

Issues
------

Bugs and feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/simple_timeline/issues.

Credits
-------

- Originally written for Drupal by [Alan Lobo](https://github.com/alansaviolobo).
- Ported to Backdrop CMS by [Robert Garrigos](https://github.com/robertgarrigos).
- Enhanced Timeline plugin and 2.x development by [Tim Erickson](https://github.com/stpaultim).

Notes About Use of AI
---------------------

This module was developed with significant assistance from AI tools (specifically Claude by Anthropic). AI was used to generate code, plan features, and make iterative improvements throughout development.

We encourage folks to review the code and help us identify quirky things that AI might have done or places where it may not be following Backdrop CMS best practices. Pull requests and issue reports are welcome.

Installation
------------

Install this module using the official Backdrop CMS instructions at https://backdropcms.org/guide/modules

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

Current Maintainers
-------------------

- [Robert Garrigos](https://github.com/robertgarrigos)
- [Tim Erickson](https://github.com/stpaultim)
