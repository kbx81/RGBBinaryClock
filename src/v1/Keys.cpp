//
// kbx81's binary clock keys class
// ---------------------------------------------------------------------------
// (c)2017 by kbx81. See LICENSE for details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//
#include "Keys.h"


namespace kbxBinaryClock {

namespace Keys {


  // The number of defined keys
  //
  static const uint8_t cKeyCount = 6;

  // The key masks for all tested keys
  //
  static const Key cKeyMasks[cKeyCount] = {A, B, C, E, D, U};

  // The size of the key queue
  //
  static const uint8_t cKeyQueueSize = 16;

  // The initial delay until repeat starts
  //
  static const uint16_t cRepeatDelay = 2000;

  // The speed of the repeat
  //
  static const uint8_t cRepeatSpeed = 200;


  // The last bitmask of the key states
  //
  static Key _currentPressedKey = None;

  // The number of checks since a key was pressed down
  //
  static uint16_t _keyPressedTimeCount = 0;

  // Flag if we are already in repeat mode
  //
  static bool _repeatMode = false;

  // Key queue for received keys
  //
  static uint8_t _keyQueue[cKeyQueueSize];

  // The current size of the key queue
  //
  static uint8_t _currentKeyQueueSize = 0;


  // Get the current pressed key
  //
  Key currentlyPressedKey()
  {
    Hardware::buttonsRefresh();
    const uint8_t mask = Hardware::buttons();
    for (uint8_t i = 0; i < cKeyCount; ++i)
    {
      if ((mask & cKeyMasks[i]) != 0)
      {
        return cKeyMasks[i];
      }
    }
    return None;
  }


  // Add a key press to the queue.
  //
  void addKeyPress(Key key)
  {
    if (_currentKeyQueueSize < cKeyQueueSize)
    {
      _keyQueue[_currentKeyQueueSize] = key;
      ++_currentKeyQueueSize;
    }
  }


  void scanKeys()
  {
    auto key = currentlyPressedKey();
    if (_currentPressedKey != key)
    {
      _currentPressedKey = key;
      _keyPressedTimeCount = 0;
      _repeatMode = false;
      if (key != None)
      {
        addKeyPress(key);
      }
    }
    else if (_currentPressedKey != None)
    {
      ++_keyPressedTimeCount;
      if (_repeatMode && _keyPressedTimeCount > cRepeatSpeed)
      {
        _keyPressedTimeCount = 0;
        addKeyPress(_currentPressedKey);
      }
      else if (_keyPressedTimeCount > cRepeatDelay)
      {
        _keyPressedTimeCount = 0;
        addKeyPress(_currentPressedKey);
        _repeatMode = true;
      }
    }
  }


  bool hasKeyPress()
  {
    return (_currentKeyQueueSize > 0);
  }


  Key getKeyPress()
  {
    if (_currentKeyQueueSize > 0)
    {
      auto key = _keyQueue[0];
      memmove(_keyQueue, _keyQueue + 1, cKeyQueueSize - 1);
      --_currentKeyQueueSize;
      return (Key)key;
    }
    else
    {
      return None;
    }
  }


}

}
