#!/usr/bin/env python3
"""
OpenWorld RPG Game
A simple text-based RPG with player management, inventory, and combat.
"""

import random
import json
import os

class Player:
    def __init__(self, name, health=100, mana=50, level=1):
        self.name = name
        self.health = health
        self.max_health = health
        self.mana = mana
        self.max_mana = mana
        self.level = level
        self.experience = 0
        self.inventory = Inventory()
        self.position = {"x": 0, "y": 0}
        
    def take_damage(self, damage):
        self.health -= damage
        if self.health <= 0:
            self.health = 0
            return True  # Player died
        return False
    
    def heal(self, amount):
        self.health += amount
        if self.health >= self.max_health:
            self.health = self.max_health
    
    def gain_experience(self, exp):
        self.experience += exp
        # Fixed: Proper level progression with level cap
        max_level = 50  # Add reasonable level cap
        
        while self.experience >= self.level * 100 and self.level < max_level:
            exp_needed = self.level * 100
            self.experience -= exp_needed
            self.level += 1
            self.max_health += 20
            self.max_mana += 10
            self.health = self.max_health
            self.mana = self.max_mana
            print(f"Level up! You are now level {self.level}")
        
        # Cap experience if at max level
        if self.level >= max_level:
            self.experience = 0
    
    def move(self, direction, world_size=None):
        # Fixed: Added bounds checking for world boundaries
        if world_size is None:
            world_size = {"width": 100, "height": 100}  # Default world size
        
        new_x, new_y = self.position["x"], self.position["y"]
        
        if direction == "north":
            new_y += 1
        elif direction == "south":
            new_y -= 1
        elif direction == "east":
            new_x += 1
        elif direction == "west":
            new_x -= 1
        
        # Check bounds and only move if within world boundaries
        if (0 <= new_x < world_size["width"] and 0 <= new_y < world_size["height"]):
            self.position["x"] = new_x
            self.position["y"] = new_y
            return True
        else:
            print(f"Cannot move {direction} - you've reached the edge of the world!")
            return False

class Inventory:
    def __init__(self):
        self.items = {}
        self.max_capacity = 20
    
    def add_item(self, item_name, quantity=1):
        if item_name in self.items:
            self.items[item_name] += quantity
        else:
            self.items[item_name] = quantity
        
        # Check if inventory is over capacity
        total_items = sum(self.items.values())
        if total_items > self.max_capacity:
            print(f"Inventory full! Dropping {item_name}")
            self.items[item_name] -= quantity
            if self.items[item_name] <= 0:
                del self.items[item_name]
    
    def remove_item(self, item_name, quantity=1):
        if item_name in self.items:
            self.items[item_name] -= quantity
            if self.items[item_name] <= 0:
                del self.items[item_name]
            return True
        return False
    
    def get_item_count(self, item_name):
        return self.items.get(item_name, 0)

class Enemy:
    def __init__(self, name, health, damage, experience_reward):
        self.name = name
        self.health = health
        self.max_health = health
        self.damage = damage
        self.experience_reward = experience_reward
    
    def attack(self, target):
        damage = random.randint(self.damage // 2, self.damage)
        return target.take_damage(damage)
    
    def take_damage(self, damage):
        self.health -= damage
        return self.health <= 0

class Game:
    def __init__(self):
        self.player = None
        self.running = True
        self.world_size = {"width": 100, "height": 100}
        
    def create_player(self, name):
        self.player = Player(name)
        print(f"Welcome, {name}! Your adventure begins...")
    
    def save_game(self, filename):
        # Fixed: Added security validation for filename
        # Sanitize filename to prevent path traversal and other attacks
        import re
        
        # Remove any path separators and dangerous characters
        safe_filename = re.sub(r'[<>:"/\\|?*]', '_', filename)  # Replace dangerous chars
        safe_filename = re.sub(r'\.\.', '_', safe_filename)      # Remove path traversal
        safe_filename = safe_filename.strip()                    # Remove leading/trailing spaces
        
        # Ensure filename is not empty and add .save extension if needed
        if not safe_filename:
            safe_filename = "savegame"
        if not safe_filename.endswith('.save'):
            safe_filename += '.save'
        
        # Restrict to saves directory for additional security
        saves_dir = "saves"
        if not os.path.exists(saves_dir):
            os.makedirs(saves_dir)
        
        safe_path = os.path.join(saves_dir, safe_filename)
        
        # Additional check to ensure we're still in the saves directory
        if not os.path.abspath(safe_path).startswith(os.path.abspath(saves_dir)):
            print("Error: Invalid filename for security reasons")
            return False
        
        game_data = {
            "player_name": self.player.name,
            "health": self.player.health,
            "mana": self.player.mana,
            "level": self.player.level,
            "experience": self.player.experience,
            "position": self.player.position,
            "inventory": self.player.inventory.items
        }
        
        try:
            with open(safe_path, 'w') as f:
                json.dump(game_data, f)
            print(f"Game saved to {safe_path}")
            return True
        except Exception as e:
            print(f"Error saving game: {e}")
            return False
    
    def load_game(self, filename):
        # Apply same security validation as save_game
        import re
        
        # Sanitize filename
        safe_filename = re.sub(r'[<>:"/\\|?*]', '_', filename)
        safe_filename = re.sub(r'\.\.', '_', safe_filename)
        safe_filename = safe_filename.strip()
        
        if not safe_filename:
            print("Error: Invalid filename")
            return False
            
        if not safe_filename.endswith('.save'):
            safe_filename += '.save'
        
        saves_dir = "saves"
        safe_path = os.path.join(saves_dir, safe_filename)
        
        # Security check
        if not os.path.abspath(safe_path).startswith(os.path.abspath(saves_dir)):
            print("Error: Invalid filename for security reasons")
            return False
        
        try:
            with open(safe_path, 'r') as f:
                game_data = json.load(f)
            
            self.player = Player(
                game_data["player_name"],
                game_data["health"],
                game_data["mana"],
                game_data["level"]
            )
            self.player.experience = game_data["experience"]
            self.player.position = game_data["position"]
            self.player.inventory.items = game_data["inventory"]
            
            print(f"Game loaded from {safe_path}")
            return True
        except FileNotFoundError:
            print(f"Save file {safe_path} not found!")
            return False
        except json.JSONDecodeError:
            print(f"Invalid save file format!")
            return False
        except Exception as e:
            print(f"Error loading game: {e}")
            return False
    
    def combat(self, enemy):
        print(f"Combat started with {enemy.name}!")
        
        while enemy.health > 0 and self.player.health > 0:
            # Player attacks
            player_damage = random.randint(10, 20) + (self.player.level * 2)
            enemy_died = enemy.take_damage(player_damage)
            print(f"You deal {player_damage} damage to {enemy.name}")
            
            if enemy_died:
                print(f"{enemy.name} defeated!")
                self.player.gain_experience(enemy.experience_reward)
                print(f"You gained {enemy.experience_reward} experience!")
                return True
            
            # Enemy attacks
            enemy.attack(self.player)
            print(f"{enemy.name} attacks you!")
            
            if self.player.health <= 0:
                print("You have been defeated!")
                return False
        
        return True
    
    def encounter_enemy(self):
        enemies = [
            Enemy("Goblin", 30, 15, 25),
            Enemy("Orc", 50, 25, 40),
            Enemy("Skeleton", 40, 20, 35)
        ]
        
        enemy = random.choice(enemies)
        print(f"A wild {enemy.name} appears!")
        
        choice = input("Do you want to (f)ight or (r)un? ").lower()
        if choice == 'f':
            return self.combat(enemy)
        else:
            print("You run away!")
            return True
    
    def main_loop(self):
        print("OpenWorld RPG Game")
        print("==================")
        
        name = input("Enter your character name: ")
        self.create_player(name)
        
        while self.running:
            print(f"\n{self.player.name} - Level {self.player.level}")
            print(f"Health: {self.player.health}/{self.player.max_health}")
            print(f"Position: ({self.player.position['x']}, {self.player.position['y']})")
            print(f"Experience: {self.player.experience}")
            
            action = input("\nWhat would you like to do? (move/inventory/save/load/encounter/quit): ").lower()
            
            if action == "move":
                direction = input("Which direction? (north/south/east/west): ").lower()
                if self.player.move(direction, self.world_size):
                    print(f"You moved {direction}")
                    
                    # Random encounter chance
                    if random.random() < 0.3:
                        if not self.encounter_enemy():
                            break
            
            elif action == "inventory":
                print("Inventory:")
                for item, count in self.player.inventory.items.items():
                    print(f"  {item}: {count}")
                
                # Add some test items
                if random.random() < 0.5:
                    items = ["Health Potion", "Sword", "Shield", "Gold"]
                    item = random.choice(items)
                    self.player.inventory.add_item(item, random.randint(1, 3))
                    print(f"Found {item}!")
            
            elif action == "save":
                filename = input("Enter save filename: ")
                self.save_game(filename)
            
            elif action == "load":
                filename = input("Enter save filename: ")
                self.load_game(filename)
            
            elif action == "encounter":
                if not self.encounter_enemy():
                    break
            
            elif action == "quit":
                print("Thanks for playing!")
                self.running = False
            
            else:
                print("Invalid action!")

if __name__ == "__main__":
    game = Game()
    game.main_loop()