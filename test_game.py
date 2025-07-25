#!/usr/bin/env python3
"""
Test script to demonstrate bugs in the OpenWorld RPG game
"""

from game import Player, Game, Enemy
import os

def test_bug_1_level_overflow():
    """Test Bug 1: Level overflow and missing level cap"""
    print("=== Testing Bug 1: Level Overflow ===")
    player = Player("TestPlayer")
    
    # Simulate giving massive experience
    print(f"Initial level: {player.level}")
    player.gain_experience(999999999)  # Very large number
    print(f"Level after massive XP: {player.level}")
    print(f"Health: {player.health}, Max Health: {player.max_health}")
    print()

def test_bug_2_bounds_checking():
    """Test Bug 2: No bounds checking for movement"""
    print("=== Testing Bug 2: Movement Bounds ===")
    game = Game()
    game.create_player("TestPlayer")
    
    print(f"World size: {game.world_size}")
    print(f"Initial position: {game.player.position}")
    
    # Move way beyond world boundaries
    for _ in range(150):  # Move 150 times east (beyond world width of 100)
        game.player.move("east")
    
    print(f"Position after 150 east moves: {game.player.position}")
    print("Note: Player can move beyond world boundaries!")
    print()

def test_bug_3_filename_security():
    """Test Bug 3: Filename security vulnerability"""
    print("=== Testing Bug 3: Filename Security ===")
    game = Game()
    game.create_player("TestPlayer")
    
    # Try to save with dangerous filenames
    dangerous_filenames = [
        "../../../etc/passwd",  # Path traversal
        "/tmp/malicious_file",  # Absolute path
        "save;rm -rf /",        # Command injection attempt
        "con.txt" if os.name == 'nt' else "/dev/null"  # Windows reserved name or Unix special file
    ]
    
    for filename in dangerous_filenames:
        print(f"Attempting to save with filename: '{filename}'")
        try:
            game.save_game(filename)
            print(f"  Successfully saved to: {filename} (SECURITY RISK!)")
        except Exception as e:
            print(f"  Error: {e}")
    print()

if __name__ == "__main__":
    print("OpenWorld RPG Bug Demonstration")
    print("===============================")
    
    test_bug_1_level_overflow()
    test_bug_2_bounds_checking()
    test_bug_3_filename_security()
    
    print("All tests completed. See output above for security and logic issues.")