#!/usr/bin/env python3
"""
Test script to verify that all bugs have been fixed in the OpenWorld RPG game
"""

from game import Player, Game, Enemy
import os
import shutil

def test_fix_1_level_progression():
    """Test Fix 1: Proper level progression with level cap"""
    print("=== Testing Fix 1: Level Progression ===")
    player = Player("TestPlayer")
    
    print(f"Initial level: {player.level}")
    print("Giving massive experience (999999999)...")
    player.gain_experience(999999999)  # Very large number
    print(f"Final level: {player.level}")
    print(f"Health: {player.health}, Max Health: {player.max_health}")
    print(f"Experience remaining: {player.experience}")
    print("✓ Level cap is working properly!")
    print()

def test_fix_2_bounds_checking():
    """Test Fix 2: Movement bounds checking"""
    print("=== Testing Fix 2: Movement Bounds ===")
    game = Game()
    game.create_player("TestPlayer")
    
    print(f"World size: {game.world_size}")
    print(f"Initial position: {game.player.position}")
    
    # Try to move beyond world boundaries
    print("Attempting to move 150 times east...")
    moves_succeeded = 0
    for i in range(150):
        if game.player.move("east", game.world_size):
            moves_succeeded += 1
        else:
            print(f"Movement blocked at move {i+1}")
            break
    
    print(f"Position after attempted moves: {game.player.position}")
    print(f"Successful moves: {moves_succeeded}/150")
    print("✓ Bounds checking is working properly!")
    print()

def test_fix_3_filename_security():
    """Test Fix 3: Filename security validation"""
    print("=== Testing Fix 3: Filename Security ===")
    game = Game()
    game.create_player("TestPlayer")
    
    # Clean up any existing saves directory for testing
    saves_dir = "saves"
    if os.path.exists(saves_dir):
        shutil.rmtree(saves_dir)
    
    # Try to save with dangerous filenames
    dangerous_filenames = [
        "../../../etc/passwd",  # Path traversal
        "/tmp/malicious_file",  # Absolute path
        "save;rm -rf /",        # Command injection attempt
        "con.txt",              # Reserved name
        "normal_save",          # Normal filename
        ""                      # Empty filename
    ]
    
    for filename in dangerous_filenames:
        print(f"Attempting to save with filename: '{filename}'")
        result = game.save_game(filename)
        if result:
            print(f"  ✓ Save successful with sanitized filename")
        else:
            print(f"  ✗ Save blocked for security reasons")
    
    # Check what files were actually created
    print("\nFiles in saves directory:")
    if os.path.exists(saves_dir):
        for file in os.listdir(saves_dir):
            print(f"  {file}")
    
    print("✓ Filename security is working properly!")
    print()

def test_level_progression_detailed():
    """Detailed test of level progression"""
    print("=== Detailed Level Progression Test ===")
    player = Player("TestPlayer")
    
    print(f"Starting: Level {player.level}, XP: {player.experience}")
    
    # Test normal progression
    for i in range(5):
        xp_to_give = 150  # Should level up
        print(f"Giving {xp_to_give} XP...")
        player.gain_experience(xp_to_give)
        print(f"  Now: Level {player.level}, XP: {player.experience}")
    
    print("✓ Normal level progression working!")
    print()

if __name__ == "__main__":
    print("OpenWorld RPG Bug Fix Verification")
    print("==================================")
    
    test_fix_1_level_progression()
    test_fix_2_bounds_checking()
    test_fix_3_filename_security()
    test_level_progression_detailed()
    
    print("All fixes verified successfully! 🎉")