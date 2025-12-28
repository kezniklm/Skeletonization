import { createClient } from "redis";

const createRedisClient = () =>
  createClient({
    url: process.env.REDIS_URL ?? "redis://localhost:6379",
    socket: {
      reconnectStrategy: (retries) => {
        if (retries > 10) {
          console.error("Redis: Too many reconnection attempts, giving up");
          return new Error("Too many reconnection attempts");
        }
        const delay = Math.min(retries * 100, 3000);
        console.log(`Redis: Reconnecting in ${delay}ms (attempt ${retries})`);
        return delay;
      }
    }
  });

export type RedisClient = ReturnType<typeof createRedisClient>;

class RedisClientManager {
  private client: RedisClient | null = null;
  private connectionPromise: Promise<void> | null = null;
  private isConnected = false;

  async getClient(): Promise<RedisClient> {
    if (this.isConnected && this.client) {
      return this.client;
    }

    if (this.connectionPromise) {
      await this.connectionPromise;
      return this.client!;
    }

    this.connectionPromise = this.connect();
    await this.connectionPromise;
    return this.client!;
  }

  private async connect(): Promise<void> {
    try {
      this.client = createRedisClient();

      this.client.on("error", (err: Error) => console.error("Redis Client Error", err));
      this.client.on("connect", () => console.log("Redis: Connected"));
      this.client.on("reconnecting", () => console.log("Redis: Reconnecting..."));
      this.client.on("ready", () => console.log("Redis: Ready"));

      await this.client.connect();
      this.isConnected = true;
      console.log("Redis: Successfully connected");
    } catch (error) {
      console.error("Redis: Failed to connect", error);
      this.connectionPromise = null;
      throw error;
    }
  }

  async createSubscriberClient(): Promise<RedisClient> {
    const client = createRedisClient();
    client.on("error", (err) => console.error("Redis Subscriber Error:", err));
    await client.connect();
    console.log("Redis Subscriber: Connected");
    return client;
  }
}

export const redisClientManager = new RedisClientManager();
